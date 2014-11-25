#include "isr.h"
#include "uartbuf.h"
#include "uarterr.h"
#include "setup.h"
#include "frame.h"
#include "board.h"
#include "functions.h"
#include "psu.h"

#include <stdio.h>
#include <usart.h>
#include <timers.h>

// TIMER1 ISR vars
volatile BOOL Ticker1s = FALSE;
static BYTE retval = 0;
static BYTE cnt1 = 0;
static char kw;
static UINT idb;
static enum slaveid ids;
static unsigned char cmd;
static unsigned char seq;
static char data[16];
static char result[16];
static UINT crc;

// UART ISR vars
extern CBuffer *Uart;
volatile UartErrLog uerrlog;
static unsigned char ch;
static UINT ix = 0;
   
#pragma udata large_udata
unsigned char framehpin[32], framehpout[32];
#pragma udata

#pragma code high_vector_section=0x08

void interrupt_at_high_vector(void) {
   _asm GOTO USARTRX_isr _endasm
}
#pragma code

#pragma code low_vector_section=0x18

void interrupt_at_low_vector(void) {
   _asm GOTO Timer1_isr _endasm
}
#pragma code

#pragma interrupt USARTRX_isr
void USARTRX_isr(void) {

   if (PIR1bits.RCIF) {

      while (DataRdyUSART()) {

         if (RCSTAbits.OERR || RCSTAbits.FERR) {

            // log error
            if(RCSTAbits.OERR)
               uerrlog.uart_oerr = uerrlog.uart_oerr + 1;

            if(RCSTAbits.FERR)
               uerrlog.uart_ferr = uerrlog.uart_ferr + 1;

            RCSTAbits.CREN = 0; // Clearing CREN clears any Overrun (OERR) errors
            Nop();
            RCSTAbits.CREN = 1; // Re-enable continuous USART receive
            RCSTAbits.SPEN = 0;
            RCSTAbits.SPEN = 1;
         }

         ch = ReadUSART();

#ifndef LOOPBACK_MODE

         Uart[SLAVE_RX].data[Uart[SLAVE_RX].wrp] = ch;
         Uart[SLAVE_RX].wrp = (Uart[SLAVE_RX].wrp + 1) % Uart[SLAVE_RX].size;
         
         if (Uart[SLAVE_RX].wrp == Uart[SLAVE_RX].rdp)
            Uart[SLAVE_RX].rdp = (Uart[SLAVE_RX].rdp + 1) % Uart[SLAVE_RX].size; // full, overwrite
#else
         while (BusyUSART());
         putcUSART(ch);
#endif
      }

      PIR1bits.RCIF = 0; // reset interrupt
   }
}

#pragma interruptlow Timer1_isr
void Timer1_isr(void) {

   if (PIR1bits.TMR1IF) {
//
//         kw = frame_getkw(framehpin);
//         idb = frame_getidb(framehpin);
//         ids = frame_getids(framehpin);
//         cmd = frame_getcmd(framehpin);
//         seq = frame_getseq(framehpin);
//         crc = frame_getcrc(framehpin);
//
//         if (!frame_checkcrc(&framehpin)) { // CRC fail
//
//            frame_build(KW_ERR, idb, ids, cmd, seq, frame_getfulldata(&framehpin), &framehpout);
//
//         } else { // CRC ok
//
//            retval = hpfunc_invoke(cmd, frame_getfulldata(&framehpin), result);
//
//            if (retval == FUNC_CMD_OK) {
//
//               frame_build(KW_ACK, idb, ids, cmd, seq, result, &framehpout); // send ACK
//
//            } else {
//
//               frame_build(KW_ERR, idb, ids, cmd, seq, frame_getfulldata(&framehpin), &framehpout); // send ERR
//            }
//
//            uartbuf_putframe(SLAVE_HPTX, &framehpout); // send response on Slave.HPTX buffer
//         }
//      }

      cnt1++;

      if (cnt1 > 30) {
         Ticker1s = TRUE;
         cnt1 = 0;
      }

      PIR1bits.TMR1IF = 0; // interrupt must be cleared by software
      PIE1bits.TMR1IE = 1; // reenable the interrupt
      TMR1H = 3; // preset for timer1 MSB register
      TMR1L = 120; // preset for timer1 LSB register
   }
}

// EOF

