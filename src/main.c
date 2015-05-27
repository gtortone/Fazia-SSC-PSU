#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <usart.h>

#pragma config FOSC = HSPLL     // HS oscillator
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = NOSLP    // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware)
#pragma config BORV = 30        // Brown Out Reset Voltage bits (VBOR set to 3.0 V nominal)

#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT is controlled by SWDTEN bit of the WDTCON register)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config HFOFST = OFF     // HFINTOSC Fast Start-up (The system clock is held off until the HFINTOSC is stable.)
#pragma config MCLRE = OFF      // MCLR Pin Enable bit (MCLR pin disabled; RE3 input pin enabled)

#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config STVREN = OFF     // Stack full/underflow will not cause reset

#include "board.h"
#include "setup.h"
#include "uartbuf.h"
#include "functions.h"
#include "comb-dev.h"
#include "spi-dev.h"
#include "psu.h"
#include "utils.h"

extern UINT32 avg_voltages[9];
extern UINT32 avg_currents[7];
extern UINT32 avg_temps[2];
extern UINT16 voltages[9];
extern UINT16 currents[7];
extern UINT16 temps[2];

static BYTE alarm_cnt[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static enum ALARM_CODE alarm_state[10] = {NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE};
static BOOL shutdown = FALSE;

void do_readings(void);
void do_monitor(void);

#pragma udata large_udata
unsigned char fIn[MAX_FRAME_SIZE], fOut[MAX_FRAME_SIZE];
char data[MAX_DATA_SIZE], result[MAX_DATA_SIZE];
#pragma udata

extern UINT16 Ticker1s;
extern CBuffer *Uart;

void main(void) {

   BOOL swreset = FALSE;
   BOOL hwreset = FALSE;
   BOOL boreset = FALSE;
   BYTE retval, cmdres, funcres;

   UINT i;
   char kw;
   UINT idb;
   enum slaveid ids;
   unsigned char cmd;
   unsigned char seq;
   UINT crc, docrc;
   UINT flen, foffset;
   UINT ferr;
   unsigned char ch;
   char fpre[10];
   UINT counter;
   
   ucsetup();
   memsetup();

   swreset = !(RCONbits.RI);
   hwreset = !(RCONbits.POR);
   boreset = !(RCONbits.BOR);

   uartbuf_init();
   frame_init(fIn);
   frame_init(fOut);

   func_init();

#ifdef DEBUG

   if (hwreset)
      printf("\n\r# # # # # # # # # # # #");

   printf("\n\r\n\rProgram started - PSC \n\r");
   printf("\tSWR = %d - HWR = %d - BOR = %d\n\r", swreset, hwreset, boreset);

#endif

   // temporary solution: power on 8 FEE cards on PSU power on

   // START

   __delay_s(20);
   ps_poweron(M5);
   ps_poweron(P5);
   ps_poweron(M8);
   ps_poweron(P12);

   // check M5 and P5 voltage levels before turn on P5p5_OUT
   counter = 0;
   while(((ps_getvoltage(M5) < 4500) || (ps_getvoltage(P5) < 4500)) && (counter < 20)) {

       __delay_ms(5);
       counter++;
   }

   if(counter >= 20) {

      ps_poweroff(P12, NORMAL);
      ps_poweroff(M8, NORMAL);
      ps_poweroff(M5, NORMAL);
      ps_poweroff(P5, NORMAL);
      while(1) ;
   }

   __delay_ms(50);
   ps_poweron(P5p5_OUT);

   // END

   while (1) {

      do_readings();

#ifndef LOOPBACK_MODE

      ferr = cbuffer_getframe_length(&Uart[SLAVE_RX], &flen, &foffset);

      if (ferr == ERR_FRAME_NONE) {

         uartbuf_getframe(SLAVE_RX, fIn, flen, foffset);

         if (flen < 9) // incorrect length
            continue;

         kw = frame_getkw(fIn);
         idb = frame_getidb(fIn);
         ids = frame_getids(fIn);
         cmd = frame_getcmd(fIn);
         seq = frame_getseq(fIn);
         frame_getdata(fIn, data);
         crc = frame_getcrc(fIn);

         if ((ids == PSC)) {

            if(seq)
               sprintf(fpre, "%03X%c%c%c%c", idb, ids, cmd, seq, SEQNUM_DELIMITER);
            else
               sprintf(fpre, "%03X%c%c", idb, ids, cmd);

            docrc = 0;
            for (i = 0; fIn[i] != KW_END; i++)
               docrc = docrc ^ fIn[i];

            if ((docrc != crc) || (kw != KW_STX)) { // CRC fail

               sprintf((char *) fIn, "%c%s%s%c", KW_ERR, fpre, data, KW_END);
               crc = frame_docrc(fIn);
               sprintf((char *) fOut, "%s%X", fIn, crc);
               uartbuf_putframe(SLAVE_TX, fOut);
               continue;
            }

            cmdres = func_invoke(cmd, data, result);

            if (cmdres == FUNC_CMD_OK) {

               funcres = atoi((char *) result[0]);

               if (funcres == FUNC_EXEC_OK)
                  sprintf((char *) fIn, "%c%s%s%c", KW_ACK, fpre, result, KW_END);
               else
                  sprintf((char *) fIn, "%c%s%s%c", KW_NAK, fpre, result, KW_END);

            } else sprintf((char *) fIn, "%c%s%s%c", KW_ERR, fpre, data, KW_END);

            // compute new CRC and append to output frame
            crc = frame_docrc(fIn);
            sprintf((char *) fOut, "%s%X", fIn, crc);

            uartbuf_putframe(SLAVE_TX, fOut);
         }

      } // end if(ferr)

      uartbuf_flush(SLAVE_HPTX);
      uartbuf_flush(SLAVE_TX);

#endif

     do_monitor();

   } // end while

}

void do_readings(void) {

   static enum PSU_MODULE mod = 0;
   BYTE config;
   UINT32 TMPmed = 0;

   if (++mod > MAX_PWL_ID)
      mod = MIN_PWL_ID;

   config = ps_getconfig(mod);

   if (config & V_CONTROL) {

      TMPmed = avg_voltages[mod] >> EXP_SAMPLE;
      voltages[mod] = TMPmed;

      avg_voltages[mod] = avg_voltages[mod] - TMPmed;
      TMPmed = ps_getvoltage(mod);
      avg_voltages[mod] = avg_voltages[mod] + TMPmed;
   }

   if (config & I_CONTROL) {

      TMPmed = avg_currents[mod] >> EXP_SAMPLE;
      currents[mod] = TMPmed;

      avg_currents[mod] = avg_currents[mod] - TMPmed;
      TMPmed = ps_getcurrent(mod);
      avg_currents[mod] = avg_currents[mod] + TMPmed;
   }

   if (mod <= MAX_TMP_ID) {

      TMPmed = avg_temps[mod] >> EXP_SAMPLE;
      temps[mod] = TMPmed;

      avg_temps[mod] = avg_temps[mod] - TMPmed;
      TMPmed = ps_gettemp(mod);
      avg_temps[mod] = avg_temps[mod] + TMPmed;
   }
}

void do_monitor(void) {

   UINT16 HV_voltage = 0;
   UINT16 HV_bound = 0;
   enum PSU_STATUS PS_status, HV_status;
   enum PSU_MODULE mod, badmod;

   if (Ticker1s) { // 1 second is elapsed

      // HV control

      HV_voltage = ps_gethv_vset();
      HV_bound = ps_gethv_vbound();
      HV_status = ps_getstatus(HV);

      if ((HV_status == RAMPUP) || (HV_status == TUNEUP)) {

         HV_voltage += ps_gethv_ramprate();

         if (HV_voltage > HV_bound) {

            HV_voltage = HV_bound; // protection on increment for ramp up
            ps_setstatus(HV, UP);

         }

         ps_sethv_voltage(HV_voltage); // apply voltage level on HV

      } else {

         if ((HV_status == RAMPDOWN) || (HV_status == TUNEDOWN)) {

            if (HV_voltage > ps_gethv_ramprate()) { // protection on negative levels

               HV_voltage -= ps_gethv_ramprate();

               if ((HV_status == TUNEDOWN) && (HV_voltage < HV_bound)) {

                  HV_voltage = HV_bound;
                  ps_setstatus(HV, UP);
               }

            } else {

               HV_voltage = 0;

               if (HV_status == RAMPDOWN) {

                  ps_setstatus(HV, DOWN);
                  ps_sethv_off();
               }

            }

            ps_sethv_voltage(HV_voltage); // apply voltage level on HV
         }
      }


      // ALARM handling on card overtemperature

#ifdef DEBUG
      printf("ALARM checking T...\n\r");
#endif
      if (ps_tempalarm() == CARDOVERTEMP) {
#ifdef DEBUG
         printf("ALARM overtemp !!!!\n\r");
#endif
         ps_poweroff(P5, NORMAL);
         ps_poweroff(M5, NORMAL);
         ps_poweroff(M8, NORMAL);
         ps_poweroff(P12, NORMAL);
         ps_poweroff(P5p5_OUT, NORMAL);
         ps_poweroff(HV, NORMAL);

         ps_setalarm(P5, CARDOVERTEMP);
         ps_setalarm(M5, CARDOVERTEMP);
         ps_setalarm(M8, CARDOVERTEMP);
         ps_setalarm(P12, CARDOVERTEMP);
         ps_setalarm(P5p5_OUT, CARDOVERTEMP);
         ps_setalarm(HV, CARDOVERTEMP);
         ps_setalarm(P5p5_BC, CARDOVERTEMP);
         ps_setalarm(P22_IN, CARDOVERTEMP);
         ps_setalarm(P5p5_IN, CARDOVERTEMP);

      } else {

         // ALARM handling on voltages and currents

         for (mod = P5; mod <= P5p5_IN; mod++) {

            PS_status = ps_getstatus(mod);

            if ((PS_status != DOWN) && (PS_status != RAMPDOWN)) {

               alarm_state[mod] = ps_linealarm(mod);

               if ((alarm_state[mod] != NONE) && (alarm_state[mod] != TRIP))
                  alarm_cnt[mod]++;
               else
                  alarm_cnt[mod] = 0;

               if (alarm_cnt[mod] > MAX_ALARM_NUM) {
                  badmod = mod;
                  shutdown = TRUE;
                  break;
               }
            }

         }

         if (shutdown) { // there is a line with alarm

            shutdown = FALSE;

            if (badmod == HV)
               ps_poweroff(HV, NORMAL);

            else {

               ps_poweroff(P5, NORMAL);
               ps_poweroff(M5, NORMAL);
               ps_poweroff(M8, NORMAL);
               ps_poweroff(P12, NORMAL);
               ps_poweroff(P5p5_OUT, NORMAL);
               ps_poweroff(HV, NORMAL);

               ps_setalarm(P5, TRIP);
               ps_setalarm(M5, TRIP);
               ps_setalarm(M8, TRIP);
               ps_setalarm(P12, TRIP);
               ps_setalarm(P5p5_OUT, TRIP);
               ps_setalarm(HV, TRIP);
               ps_setalarm(P22_IN, TRIP);
               ps_setalarm(P5p5_IN, TRIP);
            }

            ps_setalarm(badmod, alarm_state[badmod]);
         }
      }

      Ticker1s = FALSE;
   } // end 1-second loop
}
// EOF
