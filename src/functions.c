#include "utils.h"
#include "functions.h"
#include "frame.h"
#include "cbuffer.h"
#include "uartbuf.h"
#include "setup.h"
#include "psu.h"
#include "comb-dev.h"
#include "spi-dev.h"
#include "uarterr.h"

#include <p18cxxx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef BYTE(*func_p)(char *, char *);

func_p hpfunc[] = {&f_reset, &f_clear, &f_echo};

func_p fplist[MAX_FUNC_NUM];

extern UINT32 avg_voltages[9];
extern UINT32 avg_currents[7];
extern UINT32 avg_temps[2];
extern UINT16 voltages[9];
extern UINT16 currents[7];
extern UINT16 temps[2];

extern CBuffer *Uart;
extern UartErrLog uerrlog;

void func_init(void) {

   fplist[0] = &f_mod_pwron; // 0x83
   fplist[1] = &f_mod_pwroff; // 0x84
   fplist[2] = &f_gettemp; //0x85
   fplist[3] = &f_getvoltage; //0x86
   fplist[4] = &f_getcurrent; //0x87
   fplist[5] = &f_sethvvbound; //0x88
   fplist[6] = &f_setvoltagethr; //0x89
   fplist[7] = &f_setcurrentthr; //0x8A
   fplist[8] = &f_getvoltagethr; //0x8B
   fplist[9] = &f_getcurrentthr; //0x8C
   fplist[10] = &f_gettempthr; //0x8D
   fplist[11] = &f_settempthr; //0x8E
   fplist[12] = &f_sethvrate; //0x8F
   fplist[13] = &f_getstatus; // 0x90
   fplist[14] = &f_getalarms; //0x91
   fplist[15] = &f_gethvlimits; //0x92
   fplist[16] = &f_alarmclear; //0x93
   fplist[17] = &f_getuerr; // 0x94
}

BYTE func_invoke(unsigned char code, char *data, char *result) {
   if (code < BASE_CMD_ID)
      return FUNC_CMD_NOT_VALID;

   if ((code - BASE_CMD_ID) >= MAX_FUNC_NUM)
      return FUNC_CMD_NOT_VALID;

   if (fplist[code - BASE_CMD_ID] == NULL)
      return FUNC_CMD_NOT_DEFINED;

   return (fplist[code - BASE_CMD_ID](data, result));
}

BYTE hpfunc_invoke(unsigned char code, char *data, char *result) {
   if (code < BASE_HPCMD_ID)
      return FUNC_CMD_NOT_VALID;

   if ((code - BASE_HPCMD_ID) >= MAX_HPFUNC_NUM)
      return FUNC_CMD_NOT_VALID;

   if (hpfunc[code - BASE_HPCMD_ID] == NULL)
      return FUNC_CMD_NOT_DEFINED;

   return (hpfunc[code - BASE_HPCMD_ID](data, result));
}

BYTE f_reset(char *data, char *result) {
   Reset();
   return FUNC_CMD_OK; // never reached...
}

BYTE f_clear(char *data, char *result) {

   BYTE retval;

   uartbuf_init();

   // reset UART error log
   uerrlog.uart_ferr = 0;
   uerrlog.uart_oerr = 0;

   retval = FUNC_EXEC_OK;
   sprintf(result, "%d|", retval);

   return FUNC_CMD_OK;
}

BYTE f_echo(char *data, char *result) {

   BYTE retval;

   retval = FUNC_EXEC_OK;
   sprintf(result, "%d|%s", retval, data);

   return FUNC_CMD_OK;
}

BYTE f_mod_pwron(char *data, char *result) {

   BYTE retval;
   BYTE channel;

   if (strlen(data) == 0) {

      ps_poweron(P5);
      ps_poweron(M5);
      ps_poweron(M8);
      ps_poweron(P12);
      ps_poweron(P5p5_OUT);

      sprintf(result, "%d|", FUNC_EXEC_OK);

   } else {

      channel = atoi(data);
      retval = ps_poweron(channel);

      if (retval) // MODULE_NOT_VALID
         sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE); // str dest, formato, variabili
      else
         sprintf(result, "%d|", FUNC_EXEC_OK);
   }

   return FUNC_CMD_OK;
}

BYTE f_mod_pwroff(char *data, char *result) {

   BYTE retval;
   BYTE channel;

   if (strlen(data) == 0) {

      ps_poweroff(P5, NORMAL);
      ps_poweroff(M5, NORMAL);
      ps_poweroff(M8, NORMAL);
      ps_poweroff(P12, NORMAL);
      ps_poweroff(P5p5_OUT, NORMAL);

      sprintf(result, "%d|", FUNC_EXEC_OK);

   } else {

      channel = atoi(data);
      retval = ps_poweroff(channel, NORMAL);

      if (retval) // MODULE_NOT_VALID
         sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE); // str dest, formato, variabili
      else
         sprintf(result, "%d|", FUNC_EXEC_OK);
   }

   return FUNC_CMD_OK;
}

BYTE f_gettemp(char *data, char *result) {

   BYTE line;
   UINT16 LOCAL_int, HB_int;
   UINT16 LOCAL_dec, HB_dec;
   
   LOCAL_int = temps[LOCAL] / 10;
   LOCAL_dec = temps[LOCAL] - (LOCAL_int * 10);

   HB_int = temps[HB] / 10;
   HB_dec = temps[HB] - (HB_int * 10);

   if (strlen(data) == 0) {

      sprintf(result, "%d|%u.%02u,%u.%02u", FUNC_EXEC_OK, LOCAL_int, LOCAL_dec, HB_int, HB_dec);

   } else {

      line = atoi(data);

      if (line == LOCAL)
         sprintf(result, "%d|%u.%02u", FUNC_EXEC_OK, LOCAL_int, LOCAL_dec);
      else if (line == HB)
         sprintf(result, "%d|%u.%02u", FUNC_EXEC_OK, HB_int, HB_dec);
      else
         sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
   }

   return FUNC_CMD_OK;
}

BYTE f_getvoltage(char *data, char *result) {

   BYTE line;
   UINT16 line_int, line_dec;
   UINT16 P5_int, M5_int, M12_int, P12_int, P5p5_OUT_int, HV_int, P22_IN_int, P5p5_IN_int;
   UINT16 P5_dec, M5_dec, M12_dec, P12_dec, P5p5_OUT_dec, HV_dec, P22_IN_dec, P5p5_IN_dec;

   P5_int = voltages[P5] / 1000;
   P5_dec = (voltages[P5] - (P5_int * 1000)) / 10;

   M5_int = voltages[M5] / 1000;
   M5_dec = (voltages[M5] - (M5_int * 1000)) / 10;

   M12_int = voltages[M8] / 1000;
   M12_dec = (voltages[M8] - (M12_int * 1000)) / 10;

   P12_int = voltages[P12] / 1000;
   P12_dec = (voltages[P12] - (P12_int * 1000)) / 10;

   P5p5_OUT_int = voltages[P5p5_OUT] / 1000;
   P5p5_OUT_dec = (voltages[P5p5_OUT] - (P5p5_OUT_int * 1000)) / 10;

   HV_int = voltages[HV] / 100;
   HV_dec = (voltages[HV] - (HV_int * 100));

   P22_IN_int = voltages[P22_IN] / 1000;
   P22_IN_dec = (voltages[P22_IN] - (P22_IN_int * 1000)) / 10;

   P5p5_IN_int = voltages[P5p5_IN] / 1000;
   P5p5_IN_dec = (voltages[P5p5_IN] - (P5p5_IN_int * 1000)) / 10;

   if (strlen(data) == 0) {

      sprintf(result, "%d|%u.%02u,%u.%02u,%u.%02u,%u.%02u,%u.%02u,%u.%02u,%u.%02u,%u.%02u",
              FUNC_EXEC_OK,
              P5_int, P5_dec,
              M5_int, M5_dec,
              M12_int, M12_dec,
              P12_int, P12_dec,
              P5p5_OUT_int, P5p5_OUT_dec,
              HV_int, HV_dec,
              P22_IN_int, P22_IN_dec,
              P5p5_IN_int, P5p5_IN_dec);

   } else {

      line = atoi(data);

      if ((line >= 0) && (line <= 8) && (line != P5p5_BC)) {

         if (line == HV) {
            line_int = voltages[line] / 100;
            line_dec =  (voltages[line] - (line_int * 100));
         } else {
            line_int = voltages[line] / 1000;
            line_dec =  (voltages[line] - (line_int * 1000)) / 10;
         }

         sprintf(result, "%d|%u.%02u", FUNC_EXEC_OK, line_int, line_dec);
      } else
         sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
   }

   return FUNC_CMD_OK;
}

BYTE f_getcurrent(char *data, char *result) {

   BYTE line;

   if (strlen(data) == 0) {

      sprintf(result, "%d|%u,%u,%u,%u,%u,%u,%u", FUNC_EXEC_OK, currents[P5], currents[M5],
              currents[M8], currents[P12], currents[P5p5_OUT], currents[HV], currents[P5p5_BC]);

   } else {

      line = atoi(data);

      if ((line >= 0) && (line <= 6))
         sprintf(result, "%d|%u", FUNC_EXEC_OK, currents[line]); // %u is ok to print UINT16
      else
         sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
   }

   return FUNC_CMD_OK;
}

BYTE f_sethvvbound(char *data, char *result) {

   UINT16 value;

   value = atoi(data);

   if ((value >= 0) && (value <= ps_gethv_vabsmax())) {

      sprintf(result, "%d|", FUNC_EXEC_OK);
      ps_sethv_vbound(value);

   } else {

      sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
   }

   return FUNC_CMD_OK;
}

BYTE f_setvoltagethr(char *data, char *result) {

   BYTE i, k;
   BYTE comma_ix[2] = {0, 0};
   BYTE dlen = strlen(data);
   char line_str[4], vmin_str[9], vmax_str[9];
   BYTE line;
   UINT16 vmin, vmax;

   k = 0;
   for (i = 0; i <= dlen; i++) {

      if (data[i] == ',')
         comma_ix[k++] = i;
   }

   if (k != 2) {
      sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
      return FUNC_CMD_OK;
   }

   strncpy(line_str, data, comma_ix[0]);
   line_str[comma_ix[0]] = '\0';

   strncpy(vmin_str, data + comma_ix[0] + 1, comma_ix[1] - comma_ix[0]);
   vmin_str[comma_ix[1] - comma_ix[0]] = '\0';

   strncpy(vmax_str, data + comma_ix[1] + 1, dlen - comma_ix[0]);
   vmax_str[dlen - comma_ix[0]] = '\0';

   line = atoi(line_str);
   vmin = atoi(vmin_str);
   vmax = atoi(vmax_str);

   if ((vmax <= vmin) || (line < 0) || (line > 8) || (line == P5p5_BC)) {
      sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
      return FUNC_CMD_OK;
   }

   ps_setvoltage_thr(line, vmin, vmax);
   sprintf(result, "%d|", FUNC_EXEC_OK);
   ps_memwr_vthr(line, vmin, vmax);

   return FUNC_CMD_OK;
}

BYTE f_setcurrentthr(char *data, char *result) {

   BYTE i;
   BYTE comma_ix = 0;
   BYTE dlen = strlen(data);
   char line_str[4], imax_str[9];
   BYTE line;
   UINT16 imax;

   for (i = 0; i <= dlen; i++) {

      if (data[i] == ',')
         comma_ix = i;
   }

   if (!comma_ix) {
      sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
      return FUNC_CMD_OK;
   }

   strncpy(line_str, data, comma_ix);
   line_str[comma_ix] = '\0';

   strncpy(imax_str, data + comma_ix + 1, dlen - comma_ix);
   imax_str[dlen - comma_ix] = '\0';

   line = atoi(line_str);
   imax = atoi(imax_str);

   if ((line < 0) || (line > 6) || (imax > ps_getcurrent_absmax(line))) {
      sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
      return FUNC_CMD_OK;
   }

   ps_setcurrent_thr(line, imax);
   sprintf(result, "%d|", FUNC_EXEC_OK);
   ps_memwr_cthr(line, imax);

   return FUNC_CMD_OK;
}

BYTE f_getvoltagethr(char *data, char *result) {

   BYTE line;

   line = atoi(data);

   // check V_CONTROL
   if ((line >= 0) && (line <= 8) && (line != P5p5_BC))
      sprintf(result, "%d|%u,%u", FUNC_EXEC_OK, ps_getvoltage_minthr(line), ps_getvoltage_maxthr(line));
   else
      sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);

   return FUNC_CMD_OK;
}

BYTE f_getcurrentthr(char *data, char *result) {

   BYTE line;

   line = atoi(data);

   // check I_CONTROL
   if ((line >= 0) && (line <= 6))
      sprintf(result, "%d|%u", FUNC_EXEC_OK, ps_getcurrent_maxthr(line));
   else
      sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);

   return FUNC_CMD_OK;
}

BYTE f_settempthr(char *data, char *result) {

   BYTE i;
   BYTE comma_ix = 0;
   BYTE dlen = strlen(data);
   char line_str[4], tmax_str[8];
   BYTE line;
   UINT16 tmax;

   for (i = 0; i <= dlen; i++) {

      if (data[i] == ',')
         comma_ix = i;
   }

   if (!comma_ix) {
      sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
      return FUNC_CMD_OK;
   }

   strncpy(line_str, data, comma_ix);
   line_str[comma_ix] = '\0';

   strncpy(tmax_str, data + comma_ix + 1, dlen - comma_ix);
   tmax_str[dlen - comma_ix] = '\0';

   line = atoi(line_str);
   tmax = atoi(tmax_str);

   if ((line < 0) || (line > 1)) {
      sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
      return FUNC_CMD_OK;
   }

   ps_settemp_thr(line, tmax);
   sprintf(result, "%d|", FUNC_EXEC_OK);
   ps_memwr_tthr(line, tmax);

   return FUNC_CMD_OK;
}

BYTE f_gettempthr(char *data, char *result) {

   BYTE line;

   line = atoi(data);

   // check temperature sensor id
   if ((line >= 0) && (line <= 1))
      sprintf(result, "%d|%u", FUNC_EXEC_OK, ps_gettemp_thr(line));
   else
      sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);

   return FUNC_CMD_OK;
}

BYTE f_sethvrate(char *data, char *result) {

   UINT16 value;

   value = atoi(data);

   if ((value >= 0) && (value <= 10000)) {

      sprintf(result, "%d|", FUNC_EXEC_OK);
      ps_sethv_ramprate(value);

   } else {

      sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
   }

   return FUNC_CMD_OK;
}

BYTE f_getstatus(char *data, char *result) {

   BYTE line;

   if (strlen(data) == 0) {

      sprintf(result, "%d|%d,%d,%d,%d,%d,%d,%d,%d,%d", FUNC_EXEC_OK, ps_getstatus(P5), ps_getstatus(M5),
              ps_getstatus(M8), ps_getstatus(P12), ps_getstatus(P5p5_OUT), ps_getstatus(HV),
              ps_getstatus(P5p5_BC), ps_getstatus(P22_IN), ps_getstatus(P5p5_IN));

   } else {

      line = atoi(data);

      if ((line >= 0) && (line <= 8))
         sprintf(result, "%d|%d", FUNC_EXEC_OK, ps_getstatus(line));
      else
         sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
   }

   return FUNC_CMD_OK;
}

BYTE f_getalarms(char *data, char *result) {

   BYTE line;

   if (strlen(data) == 0) {

      sprintf(result, "%d|%d,%d,%d,%d,%d,%d,%d,%d,%d", FUNC_EXEC_OK, ps_getalarm(P5), ps_getalarm(M5),
              ps_getalarm(M8), ps_getalarm(P12), ps_getalarm(P5p5_OUT), ps_getalarm(HV),
              ps_getalarm(P5p5_BC), ps_getalarm(P22_IN), ps_getalarm(P5p5_IN));

   } else {

      line = atoi(data);

      if ((line >= 0) && (line <= 8))
         sprintf(result, "%d|%d", FUNC_EXEC_OK, ps_getstatus(line));
      else
         sprintf(result, "%d|", FUNC_EXEC_BAD_ARGS_VALUE);
   }

   return FUNC_CMD_OK;
}

BYTE f_gethvlimits(char *data, char *result) {

   sprintf(result, "%d|%u,%u,%u", FUNC_EXEC_OK, ps_gethv_ramprate(), ps_gethv_ramprate(), ps_gethv_vbound());

   return FUNC_CMD_OK;
}

BYTE f_alarmclear(char *data, char *result) {

   ps_setalarm(P5, NONE);
   ps_setalarm(M5, NONE);
   ps_setalarm(M8, NONE);
   ps_setalarm(P12, NONE);
   ps_setalarm(P5p5_OUT, NONE);
   ps_setalarm(HV, NONE);
   ps_setalarm(P5p5_BC, NONE);
   ps_setalarm(P22_IN, NONE);
   ps_setalarm(P5p5_IN, NONE);

   sprintf(result, "%d|", FUNC_EXEC_OK);

   return FUNC_CMD_OK;
}

BYTE f_getuerr(char *data, char *result) {

   static UINT16 counter = 0;

   sprintf(result, "%d|U-FERR=%d,U-OERR=%d,counter=%d", FUNC_EXEC_OK, uerrlog.uart_ferr, uerrlog.uart_oerr, counter++);

   return FUNC_CMD_OK;
}

// EOF
