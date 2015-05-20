#include <stdio.h>
#include <p18cxxx.h>
#include <EEP.h>

#include "psu.h"
#include "comb-dev.h"
#include "spi-dev.h"
#include "utils.h"

#pragma udata large_udata
static psu_t psu;
static psu_t *PSU = &psu;
#pragma udata

UINT32 avg_voltages[9] = {0, 0, 0, 0, 0, 0, 0, 0}; // lines: +5V, -5V, -8V, +12V, +5.5Vout, -600V, +22Vin, +5.5in
UINT32 avg_currents[7] = {0, 0, 0, 0, 0, 0, 0}; // lines: +5V, -5V, -8V, +12V, +5.5Vout, -600V, +5.5V_BC
UINT32 avg_temps[2] = {0, 0};
UINT16 voltages[9] = {0, 0, 0, 0, 0, 0, 0, 0};
UINT16 currents[7] = {0, 0, 0, 0, 0, 0, 0};
UINT16 temps[2] = {0, 0};

void psu_init(void) {

   PSU->line[P5].config = V_CONTROL | I_CONTROL | HAS_SWITCH;
   PSU->line[P5].swport = &LATA;
   PSU->line[P5].swpin = 4; // LATA4
   PSU->line[P5].vmuxch = 4;
   PSU->line[P5].vcoeff = 7009;
   PSU->line[P5].imuxch = 10;
   PSU->line[P5].icoeff = 4412;
   PSU->line[P5].Vnorm = 5000;

   if ( !ps_memrd_vthr(P5, &(PSU->line[P5].Vmin), &(PSU->line[P5].Vmax)) ) {
      PSU->line[P5].Vmin = PSU->line[P5].Vnorm - (PSU->line[P5].Vnorm / 100 * GAP_RATE);
      PSU->line[P5].Vmax = PSU->line[P5].Vnorm + (PSU->line[P5].Vnorm / 100 * GAP_RATE);
   }

   PSU->line[P5].Iabsmax = 3000;
   if ( !ps_memrd_cthr(P5, &(PSU->line[P5].Imax)) )
      PSU->line[P5].Imax = PSU->line[P5].Iabsmax;

   PSU->line[P5].status = DOWN;
   PSU->line[P5].alarm = NONE;

   //

   PSU->line[M5].config = V_CONTROL | I_CONTROL | HAS_SWITCH;
   PSU->line[M5].swport = &LATA;
   PSU->line[M5].swpin = 2; // LATA2
   PSU->line[M5].vmuxch = 3;
   PSU->line[M5].vcoeff = 6960;
   PSU->line[M5].imuxch = 9;
   PSU->line[M5].icoeff = 2970;
   PSU->line[M5].Vnorm = 5000;

   if ( !ps_memrd_vthr(M5, &(PSU->line[M5].Vmin), &(PSU->line[M5].Vmax)) ) {
      PSU->line[M5].Vmin = PSU->line[M5].Vnorm - (PSU->line[M5].Vnorm / 100 * GAP_RATE);
      PSU->line[M5].Vmax = PSU->line[M5].Vnorm + (PSU->line[M5].Vnorm / 100 * GAP_RATE);
   }

   PSU->line[M5].Iabsmax = 2000;
   if ( !ps_memrd_cthr(M5, &(PSU->line[M5].Imax)) )
      PSU->line[M5].Imax = PSU->line[M5].Iabsmax;

   PSU->line[M5].status = DOWN;
   PSU->line[M5].alarm = NONE;

   //

   PSU->line[M8].config = V_CONTROL | I_CONTROL | HAS_SWITCH;
   PSU->line[M8].swport = &LATA;
   PSU->line[M8].swpin = 1; // LATA1
   PSU->line[M8].vmuxch = 2;
   PSU->line[M8].vcoeff = 10310;
   PSU->line[M8].imuxch = 8;
   PSU->line[M8].icoeff = 1466;
   PSU->line[M8].Vnorm = 9500;      // 8000

   if ( !ps_memrd_vthr(M8, &(PSU->line[M8].Vmin), &(PSU->line[M8].Vmax)) ) {
      PSU->line[M8].Vmin = PSU->line[M8].Vnorm - (PSU->line[M8].Vnorm / 100 * GAP_RATE);
      PSU->line[M8].Vmax = PSU->line[M8].Vnorm + (PSU->line[M8].Vnorm / 100 * GAP_RATE);
   }

   PSU->line[M8].Iabsmax = 900;
   if ( !ps_memrd_cthr(M8, &(PSU->line[M8].Imax)) )
      PSU->line[M8].Imax = PSU->line[M8].Iabsmax;

   PSU->line[M8].status = DOWN;
   PSU->line[M8].alarm = NONE;

   //

   PSU->line[P12].config = V_CONTROL | I_CONTROL | HAS_SWITCH;
   PSU->line[P12].swport = &LATC;
   PSU->line[P12].swpin = 1; // LATC1
   PSU->line[P12].vmuxch = 1;
   PSU->line[P12].vcoeff = 16110;
   PSU->line[P12].imuxch = 7;
   PSU->line[P12].icoeff = 2000;
   PSU->line[P12].Vnorm = 14000;  

   if ( !ps_memrd_vthr(P12, &(PSU->line[P12].Vmin), &(PSU->line[P12].Vmax)) ) {
      PSU->line[P12].Vmin = PSU->line[P12].Vnorm - (PSU->line[P12].Vnorm / 100 * GAP_RATE);
      PSU->line[P12].Vmax = PSU->line[P12].Vnorm + (PSU->line[P12].Vnorm / 100 * GAP_RATE);
   }

   PSU->line[P12].Iabsmax = 1600;
   if ( !ps_memrd_cthr(P12, &(PSU->line[P12].Imax)) )
      PSU->line[P12].Imax = PSU->line[P12].Iabsmax;

   PSU->line[P12].status = DOWN;
   PSU->line[P12].alarm = NONE;

   //

   PSU->line[P5p5_OUT].config = V_CONTROL | I_CONTROL | HAS_SWITCH;
   PSU->line[P5p5_OUT].swport = &LATA;
   PSU->line[P5p5_OUT].swpin = 5; // LATA5
   PSU->line[P5p5_OUT].vmuxch = 5;
   PSU->line[P5p5_OUT].vcoeff = 7412;
   PSU->line[P5p5_OUT].imuxch = 11;
   PSU->line[P5p5_OUT].icoeff = 60000;
   PSU->line[P5p5_OUT].Vnorm = 5500;

   if ( !ps_memrd_vthr(P5p5_OUT, &(PSU->line[P5p5_OUT].Vmin), &(PSU->line[P5p5_OUT].Vmax)) ) {
      PSU->line[P5p5_OUT].Vmin = PSU->line[P5p5_OUT].Vnorm - (PSU->line[P5p5_OUT].Vnorm / 100 * GAP_RATE);
      PSU->line[P5p5_OUT].Vmax = PSU->line[P5p5_OUT].Vnorm + (PSU->line[P5p5_OUT].Vnorm / 100 * GAP_RATE);
   }

   PSU->line[P5p5_OUT].Iabsmax = 40000;     // for V3 FEE cards
   if ( !ps_memrd_cthr(P5p5_OUT, &(PSU->line[P5p5_OUT].Imax)) )
      PSU->line[P5p5_OUT].Imax = PSU->line[P5p5_OUT].Iabsmax;

   PSU->line[P5p5_OUT].status = DOWN;
   PSU->line[P5p5_OUT].alarm = NONE;

   //

   PSU->line[P5p5_BC].config = I_CONTROL;
   PSU->line[P5p5_BC].imuxch = 14;
   PSU->line[P5p5_BC].icoeff = 5357;

   PSU->line[P5p5_BC].Iabsmax = 4000;
   if ( !ps_memrd_cthr(P5p5_BC, &(PSU->line[P5p5_BC].Imax)) )
      PSU->line[P5p5_BC].Imax = PSU->line[P5p5_BC].Iabsmax;

   PSU->line[P5p5_BC].status = UP;
   PSU->line[P5p5_BC].alarm = NONE;

   //

   PSU->line[P22_IN].config = V_CONTROL;
   PSU->line[P22_IN].vmuxch = 13;
   PSU->line[P22_IN].vcoeff = 28590;
   PSU->line[P22_IN].Vnorm = 22000;

   if ( !ps_memrd_vthr(P22_IN, &(PSU->line[P22_IN].Vmin), &(PSU->line[P22_IN].Vmax)) ) {
      PSU->line[P22_IN].Vmin = PSU->line[P22_IN].Vnorm - (PSU->line[P22_IN].Vnorm / 100 * GAP_RATE);
      PSU->line[P22_IN].Vmax = PSU->line[P22_IN].Vnorm + (PSU->line[P22_IN].Vnorm / 100 * GAP_RATE);
   }

   PSU->line[P22_IN].status = UP;
   PSU->line[P22_IN].alarm = NONE;

   //

   PSU->line[P5p5_IN].config = V_CONTROL;
   PSU->line[P5p5_IN].vmuxch = 12;
   PSU->line[P5p5_IN].vcoeff = 7140;
   PSU->line[P5p5_IN].Vnorm = 5500;

   if ( !ps_memrd_vthr(P5p5_IN, &(PSU->line[P5p5_IN].Vmin), &(PSU->line[P5p5_IN].Vmax)) ) {
      PSU->line[P5p5_IN].Vmin = PSU->line[P5p5_IN].Vnorm - (PSU->line[P5p5_IN].Vnorm / 100 * GAP_RATE);
      PSU->line[P5p5_IN].Vmax = PSU->line[P5p5_IN].Vnorm + (PSU->line[P5p5_IN].Vnorm / 100 * GAP_RATE);
   }

   PSU->line[P5p5_IN].status = UP;
   PSU->line[P5p5_IN].alarm = NONE;

   //

   PSU->hv.config = V_CONTROL | I_CONTROL | HAS_SWITCH;
   PSU->hv.swport = &LATC;
   PSU->hv.swpin = 0; // LATC0
   PSU->hv.vmuxch = 0;
   PSU->hv.vcoeff = 64000;
   PSU->hv.imuxch = 6;
   PSU->hv.icoeff = 2449;
   PSU->hv.Vset = 0;
   PSU->hv.Vbound = 0;

   if ( !ps_memrd_vthr(HV, &(PSU->hv.Vmin), &(PSU->hv.Vmax)) ) {
      PSU->hv.Vmin = 0;
      PSU->hv.Vmax = 500;
   }

   PSU->hv.Iabsmax = 2000; // in uA - (2000 uA = 2 mA)
   if ( !ps_memrd_cthr(HV, &(PSU->hv.Imax)) )
      PSU->hv.Imax = PSU->hv.Iabsmax;

   PSU->hv.Vabsmax = 500;
   PSU->hv.status = DOWN;
   PSU->hv.alarm = NONE;
   PSU->hv.Vrampup = 10; // default value for rampup 10 V/s
   PSU->hv.Vrampdown = 10; // default value for rampdown 10 V/s

   //

   PSU->temps[LOCAL].vcoeff = 3000;
   PSU->temps[LOCAL].offset = 204;
   if( !ps_memrd_tthr(LOCAL, &(PSU->temps[LOCAL].Tmax)) )
      PSU->temps[LOCAL].Tmax = 50;

   //

   PSU->temps[HB].vmuxch = 15;
   PSU->temps[HB].vcoeff = 3000;
   PSU->temps[HB].offset = 13107;
   if( !ps_memrd_tthr(HB, &(PSU->temps[HB].Tmax)) )
      PSU->temps[HB].Tmax = 50;
}

BYTE ps_poweron(enum PSU_MODULE mod) {

   if((mod < MIN_PWL_ID) || (mod > MAX_PWL_ID))
      return (MODULE_NOT_VALID);
   
   if ((mod != HV) && !(PSU->line[mod].config & HAS_SWITCH))
      return (MODULE_FEATURE_NA);

   if ((ps_getstatus(mod) != UP) && (ps_getalarm(mod) == NONE)) {

      if (mod == HV) {
         ps_sethv_on();
         ps_setstatus(HV, RAMPUP);
         return (MODULE_OK);
      }

      if (mod == P5p5_OUT) {
         bitclr(*(PSU->line[mod].swport), PSU->line[mod].swpin);
         ps_setstatus(P5p5_OUT, UP);
         return (MODULE_OK);
      }

      bitset(*(PSU->line[mod].swport), PSU->line[mod].swpin);
      ps_setstatus(mod, UP);
   }

   return (MODULE_OK);
}

BYTE ps_poweroff(enum PSU_MODULE mod, UINT force) {

   if((mod < MIN_PWL_ID) || (mod > MAX_PWL_ID))
      return (MODULE_NOT_VALID);

   if ((mod != HV) && !(PSU->line[mod].config & HAS_SWITCH))
      return (MODULE_FEATURE_NA);

   if ((ps_getstatus(mod) != DOWN) || (force)) {

      if (mod == HV) {
         ps_setstatus(HV, RAMPDOWN);
         return (MODULE_OK);
      }

      if (mod == P5p5_OUT) {
         bitset(*(PSU->line[mod].swport), PSU->line[mod].swpin);
         ps_setstatus(P5p5_OUT, DOWN);
         return (MODULE_OK);
      }

      bitclr(*(PSU->line[mod].swport), PSU->line[mod].swpin);
      ps_setstatus(mod, DOWN);
   }

   return (MODULE_OK);
}

BYTE ps_getconfig(enum PSU_MODULE mod) {

   if(mod == HV)
      return(PSU->hv.config);

   return(PSU->line[mod].config);
}

BOOL ps_memrd_vthr(enum PSU_MODULE mod, UINT16 *Vmin, UINT16 *Vmax) {

   UINT16 vthr_baddr;

   if ((mod != HV) && !(PSU->line[mod].config & V_CONTROL))
      return FALSE;

   vthr_baddr = EEP_BASE_ADDR + (MAX_PWL_ID * mod);

   if (ReadEEPROM(vthr_baddr) != EEP_DATA_VALID)
      return FALSE;

   *Vmin =ReadEEPROM(vthr_baddr + 1);
   *Vmin = (*Vmin << 8) & 0xFF00;
   *Vmin = *Vmin + ReadEEPROM(vthr_baddr + 2);

   *Vmax = ReadEEPROM(vthr_baddr + 3);
   *Vmax = (*Vmax << 8) & 0xFF00;
   *Vmax = *Vmax + ReadEEPROM(vthr_baddr + 4);

   return TRUE;
}

BOOL ps_memrd_cthr(enum PSU_MODULE mod, UINT16 *Imax) {

   UINT16 cthr_baddr;

   if ((mod != HV) && !(PSU->line[mod].config & I_CONTROL))
      return FALSE;

   cthr_baddr = EEP_BASE_ADDR + (MAX_PWL_ID * mod) + 5;

   if (ReadEEPROM(cthr_baddr) != EEP_DATA_VALID)
      return FALSE;

   *Imax = ReadEEPROM(cthr_baddr + 1);
   *Imax = *Imax << 8;
   *Imax = *Imax + ReadEEPROM(cthr_baddr + 2);

   return TRUE;
}

BOOL ps_memrd_tthr(enum TSENS_MODULE mod, UINT16 *Tmax) {

   UINT16 tthr_baddr;

   // addr(0x74), addr(0x75), addr(0x76) = LOCAL max_temp threshold
   // addr(0x77), addr(0x78), addr(0x79) = HB max_temp threshold
   if (mod == LOCAL)
      tthr_baddr = EEP_BASE_ADDR + 64;

   if (mod == HB)
      tthr_baddr = EEP_BASE_ADDR + 67;

   if (ReadEEPROM(tthr_baddr) != EEP_DATA_VALID)
      return FALSE;

   *Tmax = ReadEEPROM(tthr_baddr + 1);
   *Tmax = *Tmax << 8;
   *Tmax = *Tmax + ReadEEPROM(tthr_baddr + 2);

   return TRUE;
}

void ps_memwr_vthr(enum PSU_MODULE mod, UINT16 Vmin, UINT16 Vmax) {
   
   UINT16 vthr_baddr;

   vthr_baddr = EEP_BASE_ADDR + (MAX_PWL_ID * mod);

   WriteEEPROM(vthr_baddr, EEP_DATA_VALID);
   WriteEEPROM(vthr_baddr + 1, (Vmin & 0xFF00) >> 8);
   WriteEEPROM(vthr_baddr + 2, Vmin & 0x00FF);
   WriteEEPROM(vthr_baddr + 3, (Vmax & 0xFF00) >> 8);
   WriteEEPROM(vthr_baddr + 4, Vmax & 0x00FF);
}

void ps_memwr_cthr(enum PSU_MODULE mod, UINT16 Imax) {
   
   UINT16 cthr_baddr;
   
   cthr_baddr = EEP_BASE_ADDR + (MAX_PWL_ID * mod) + 5;
   
   WriteEEPROM(cthr_baddr, EEP_DATA_VALID);
   WriteEEPROM(cthr_baddr + 1, (Imax & 0xFF00) >> 8);
   WriteEEPROM(cthr_baddr + 2, (Imax & 0x00FF));
}

void ps_memwr_tthr(enum TSENS_MODULE mod, UINT16 Tmax) {

   UINT16 tthr_baddr;

   // addr(0x74), addr(0x75), addr(0x76) = LOCAL max_temp threshold
   // addr(0x77), addr(0x78), addr(0x79) = HB max_temp threshold
   if (mod == LOCAL)
      tthr_baddr = EEP_BASE_ADDR + 64;

   if (mod == HB)
      tthr_baddr = EEP_BASE_ADDR + 67;

   WriteEEPROM(tthr_baddr, EEP_DATA_VALID);
   WriteEEPROM(tthr_baddr + 1, (Tmax & 0xFF00) >> 8);
   WriteEEPROM(tthr_baddr + 2, (Tmax & 0x00FF));
}

void ps_setstatus(enum PSU_MODULE mod, enum PSU_STATUS status) {

   if (mod == HV)
      PSU->hv.status = status;

   PSU->line[mod].status = status;
}

enum PSU_STATUS ps_getstatus(enum PSU_MODULE mod) {

   if (mod == HV)
      return (PSU->hv.status);

   return (PSU->line[mod].status);
}

UINT16 ps_gettemp(enum TSENS_MODULE ts) {

   UINT32 TMP = 0;
   UINT32 TMP1 = 0;
   UINT16 adcval = 0;
   UINT16 temp = 0;

   if ((ts < MIN_TMP_ID) || (ts > MAX_TMP_ID))
      return (MODULE_NOT_VALID);

   if (ts == LOCAL) {

      ADCON0bits.GO = 1; // start conversion
      while (ADCON0bits.DONE); //wait for the conversion to finish

      adcval = ADRESH;
      adcval = ((adcval << 8) | ADRESL);

      if (adcval > PSU->temps[ts].offset)
         adcval -= PSU->temps[ts].offset;
      else
         adcval = 0;

      TMP = adcval;
      TMP1 = TMP * PSU->temps[ts].vcoeff;

      temp = (UINT16) (TMP1 >> 10);
   }

   if (ts == HB) {

      mux_select(PSU->temps[ts].vmuxch);

      TMP = (UINT32) adc_read();
      TMP -= PSU->temps[ts].offset;
      TMP1 = TMP * (UINT32) PSU->temps[ts].vcoeff;
      temp = (UINT16) (TMP1 >> 16);
   }

   return (temp);
}

UINT16 ps_getvoltage(enum PSU_MODULE mod) {

   UINT32 TMP = 0;
   UINT32 TMP1 = 0;
   UINT16 adcval = 0;
   UINT16 voltage = 0;
   UINT16 vcoeff = 0;

   if((mod < MIN_PWL_ID) || (mod > MAX_PWL_ID))
      return (MODULE_NOT_VALID);

   if ((mod != HV) && !(PSU->line[mod].config & V_CONTROL))
      return (MODULE_FEATURE_NA);

   if (mod == HV) {

      mux_select(PSU->hv.vmuxch);
      vcoeff = PSU->hv.vcoeff;

   } else {

      mux_select(PSU->line[mod].vmuxch);
      vcoeff = PSU->line[mod].vcoeff;
   }

   TMP = adc_read();
   TMP1 = TMP * vcoeff;
   voltage = (UINT16) (TMP1 >> 16);

   return (voltage);
}

UINT16 ps_getcurrent(enum PSU_MODULE mod) {

   UINT32 TMP = 0;
   UINT32 TMP1 = 0;
   UINT16 adcval = 0;
   UINT16 current = 0;
   UINT16 current_part = 0;
   UINT16 icoeff = 0;

   if((mod < MIN_PWL_ID) || (mod > MAX_PWL_ID))
      return (MODULE_NOT_VALID);

   if ((mod != HV) && !(PSU->line[mod].config & I_CONTROL))
      return (MODULE_FEATURE_NA);

   if (mod == HV) {

      mux_select(PSU->hv.imuxch);
      icoeff = PSU->hv.icoeff;

   } else {

      mux_select(PSU->line[mod].imuxch);
      icoeff = PSU->line[mod].icoeff;
   }

   TMP = adc_read();
   TMP1 = TMP * icoeff;
   current = (UINT16) (TMP1 >> 16);

   if (mod == HV) {

      current_part = voltages[HV] / 198;

      if(current < current_part)
         current = 0;
      else
         current -= current_part;
   }

   return (current);
}

BYTE ps_setvoltage_thr(enum PSU_MODULE mod, UINT16 Vmin, UINT16 Vmax) {

   if((mod < MIN_PWL_ID) || (mod > MAX_PWL_ID))
      return (MODULE_NOT_VALID);

   if ((mod != HV) && !(PSU->line[mod].config & V_CONTROL))
      return (MODULE_FEATURE_NA);

   if (mod == HV) {

      PSU->hv.Vmin = Vmin;
      PSU->hv.Vmax = Vmax;

   } else {

      PSU->line[mod].Vmin = Vmin;
      PSU->line[mod].Vmax = Vmax;
   }

   return (MODULE_OK);
}

BYTE ps_setcurrent_thr(enum PSU_MODULE mod, UINT16 Imax) {

   if((mod < MIN_PWL_ID) || (mod > MAX_PWL_ID))
      return (MODULE_NOT_VALID);

   if ((mod != HV) && !(PSU->line[mod].config & I_CONTROL))
      return (MODULE_FEATURE_NA);

   if (mod == HV) {

      PSU->hv.Imax = Imax;

   } else {

      PSU->line[mod].Imax = Imax;
   }

   return (MODULE_OK);
}

BYTE ps_settemp_thr(enum TSENS_MODULE mod, UINT16 Tmax) {

   if ((mod < MIN_TMP_ID) || (mod > MAX_TMP_ID))
      return (MODULE_NOT_VALID);

   PSU->temps[mod].Tmax = Tmax;

   return (MODULE_OK);
}

UINT16 ps_getvoltage_minthr(enum PSU_MODULE mod) {

   if((mod < MIN_PWL_ID) || (mod > MAX_PWL_ID))
      return (MODULE_NOT_VALID);

   if ((mod != HV) && !(PSU->line[mod].config & V_CONTROL))
      return (MODULE_FEATURE_NA);

   if (mod == HV) {

      return (PSU->hv.Vmin);

   } else {

      return (PSU->line[mod].Vmin);
   }
}

UINT16 ps_getvoltage_maxthr(enum PSU_MODULE mod) {

   if((mod < MIN_PWL_ID) || (mod > MAX_PWL_ID))
      return (MODULE_NOT_VALID);

   if ((mod != HV) && !(PSU->line[mod].config & V_CONTROL))
      return (MODULE_FEATURE_NA);

   if (mod == HV) {

      return (PSU->hv.Vmax);

   } else {

      return (PSU->line[mod].Vmax);
   }
}

UINT16 ps_gethv_vabsmax(void) {

   return (PSU->hv.Vabsmax);

}

UINT16 ps_getcurrent_absmax(enum PSU_MODULE mod) {

   if((mod < MIN_PWL_ID) || (mod > MAX_PWL_ID))
      return (MODULE_NOT_VALID);

   if ((mod != HV) && !(PSU->line[mod].config & I_CONTROL))
      return (MODULE_FEATURE_NA);

   if (mod == HV) {

      return (PSU->hv.Iabsmax);

   } else {

      return (PSU->line[mod].Iabsmax);
   }
}

UINT16 ps_getcurrent_maxthr(enum PSU_MODULE mod) {

   if((mod < MIN_PWL_ID) || (mod > MAX_PWL_ID))
      return (MODULE_NOT_VALID);

   if ((mod != HV) && !(PSU->line[mod].config & I_CONTROL))
      return (MODULE_FEATURE_NA);

   if (mod == HV) {

      return (PSU->hv.Imax);

   } else {

      return (PSU->line[mod].Imax);
   }
}

UINT16 ps_gettemp_thr(enum TSENS_MODULE mod) {

   if ((mod < MIN_TMP_ID) || (mod > MAX_TMP_ID))
      return (MODULE_NOT_VALID);

   return (PSU->temps[mod].Tmax);
}

void ps_sethv_vbound(UINT16 V) {

   if (PSU->hv.status != DOWN) {

      if (V > PSU->hv.Vbound)
         ps_setstatus(HV, TUNEUP);

      if (V < PSU->hv.Vbound)
         ps_setstatus(HV, TUNEDOWN);
   }

   PSU->hv.Vbound = V;

   // if voltage threshold are not present in EEPROM then dynamic calculate new limits
   if ( !ps_memrd_vthr(HV, &(PSU->hv.Vmin), &(PSU->hv.Vmax)) ) {
      PSU->hv.Vmin = V - (((float) V / 100) * HV_GAP_RATE);
      PSU->hv.Vmax = V + (((float) V / 100) * HV_GAP_RATE);

      if( (PSU->hv.Vmax == V))
         PSU->hv.Vmax += 1;
   }
}

UINT16 ps_gethv_vbound(void) {

   return (PSU->hv.Vbound);
}

UINT16 ps_gethv_vset(void) {

   return (PSU->hv.Vset);
}

void ps_sethv_voltage(UINT16 V) {

   UINT32 value;

   PSU->hv.Vset = V;

   value = V * 109;
   value = (value >> 4) & 0x0FFF;

   dac_write(value);
}

void ps_sethv_ramprate(UINT16 rate) {

   PSU->hv.Vrampdown = PSU->hv.Vrampup = rate;
}

UINT16 ps_gethv_ramprate(void) {

   return (PSU->hv.Vrampup);
}

void ps_sethv_on(void) {
   bitset(*(PSU->hv.swport), PSU->hv.swpin);
}

void ps_sethv_off(void) {
   bitclr(*(PSU->hv.swport), PSU->hv.swpin);
}

enum ALARM_CODE ps_getalarm(enum PSU_MODULE mod) {

   if (mod == HV)
      return (PSU->hv.alarm);

   return (PSU->line[mod].alarm);
}

void ps_setalarm(enum PSU_MODULE mod, enum ALARM_CODE acode) {

   if (mod == HV)
      PSU->hv.alarm = acode;
   else
      PSU->line[mod].alarm = acode;
}

enum ALARM_CODE ps_linealarm(enum PSU_MODULE mod) {

   UINT16 Vcurr, Icurr;
   UINT16 Vmin, Vmax, Imax;
   enum PSU_STATUS PS_status;

   PS_status = ps_getstatus(mod);

   if (PS_status != DOWN) {

      if ( (PSU->line[mod].config & I_CONTROL) || (mod == HV) ) {

         Icurr = avg_currents[mod] >> EXP_SAMPLE;
         Imax = ps_getcurrent_maxthr(mod);

         if (Icurr >= Imax) {
            ps_setalarm(mod, OVERCURRENT);
            return (OVERCURRENT);
         }
      }

      if ( (PS_status == UP) && ((PSU->line[mod].config & V_CONTROL) || (mod == HV)) ) {

         Vcurr = avg_voltages[mod] >> EXP_SAMPLE;

         if (mod == HV)
            Vcurr /= 100;

         Vmin = ps_getvoltage_minthr(mod);
         Vmax = ps_getvoltage_maxthr(mod);

         if (Vcurr <= Vmin) {
            ps_setalarm(mod, UNDERVOLTAGE);
            return (UNDERVOLTAGE);
         }

         if (Vcurr >= Vmax) {
            ps_setalarm(mod, OVERVOLTAGE);
            return (OVERVOLTAGE);
         }
      }
   }

   ps_setalarm(mod, NONE);
   return (NONE);
}

enum ALARM_CODE ps_tempalarm(void) {

   UINT16 HBtemp, LOCALtemp;
   UINT16 HBtmax, LOCALtmax;

   HBtemp = ps_gettemp(HB) / 10;
   HBtmax = ps_gettemp_thr(HB);

   if (HBtemp >= HBtmax)
      return (CARDOVERTEMP);  

   LOCALtemp = ps_gettemp(LOCAL) / 10;
   LOCALtmax = ps_gettemp_thr(LOCAL);

   if (LOCALtemp >= LOCALtmax)
      return (CARDOVERTEMP);

   return (NONE);
}
// EOF
