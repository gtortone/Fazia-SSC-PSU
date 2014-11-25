#ifndef _FUNC_H
#define _FUNC_H

#include "Generic.h"

#define MAX_FUNC_NUM    18
#define MAX_HPFUNC_NUM   3

#define FUNC_CMD_OK                 0x00
#define FUNC_CMD_NOT_VALID          0x01
#define FUNC_CMD_NOT_DEFINED        0x02

#define FUNC_EXEC_OK                0x00
#define FUNC_EXEC_BAD_ARGS_TYPE     0x01
#define FUNC_EXEC_BAD_ARGS_VALUE    0x02

void func_init(void);
BYTE func_invoke(unsigned char code, char *data, char *result);
BYTE hpfunc_invoke(unsigned char code, char *data, char *result);

BYTE f_reset(char *data, char *result);
BYTE f_clear(char *data, char *result);
BYTE f_echo(char *data, char *result);

BYTE f_mod_pwron(char *data, char *result);
BYTE f_mod_pwroff(char *data, char *result);
BYTE f_gettemp(char *data, char *result);
BYTE f_getvoltage(char *data, char *result);
BYTE f_getcurrent(char *data, char *result);
BYTE f_sethvvbound(char *data, char *result);
BYTE f_sethvrate(char *data, char *result);
BYTE f_setvoltagethr(char *data, char *result);
BYTE f_setcurrentthr(char *data, char *result);
BYTE f_settempthr(char *data, char *result);
BYTE f_getvoltagethr(char *data, char *result);
BYTE f_getcurrentthr(char *data, char *result);
BYTE f_gettempthr(char *data, char *result);
BYTE f_getstatus(char *data, char *result);
BYTE f_getalarms(char *data, char *result);
BYTE f_gethvlimits(char *data, char *result);
BYTE f_alarmclear(char *data, char *result);
BYTE f_getuerr(char *data, char *result);

#endif

// EOF
