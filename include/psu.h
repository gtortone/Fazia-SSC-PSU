#ifndef _PSU_H
#define _PSU_H

#define MIN_PWL_ID           0x00   // min power line id
#define MAX_PWL_ID           0x08   // max power line id

#define MIN_TMP_ID           0x00   // min temp sensor id
#define MAX_TMP_ID           0x01   // max temp sensor id

#define MODULE_OK            0x00
#define MODULE_NOT_VALID     0x02   // (module id > MAX_PWL_ID) or (module id < MIN_PWL_ID)
#define MODULE_FEATURE_NA    0x03   // feature not available

#define V_CONTROL            0x01   // voltage control feature flag
#define I_CONTROL            0x02   // current control feature flag
#define HAS_SWITCH           0x04   // on/off switch feature flag

#define EEP_BASE_ADDR        0x10   // EEPROM base address
#define EEP_DATA_VALID       0xDA   // constant to specify valid thresholds are present in EEPROM
                                    // for a defined power line

#define GAP_RATE        10      // 10% of tolerance for voltages and currents
#define HV_GAP_RATE     20      // 20% of tolerance for high voltage line
#define MAX_ALARM_NUM   10      // maximum number of repeated alarm condition
#define EXP_SAMPLE       3      // number of sample to collect for average 2^EXP_SAMPLE = 8

#define FORCE            1      // parameter to force power off of power lines
#define NORMAL           0      // parameter to normal power off power lines

#include "Generic.h"

enum PSU_MODULE {
    P5 = 0,     // + 5.0V - 0
    M5,         // - 5.0V - 1
    M8,         // - 8.0V - 2
    P12,        // +12.0V - 3
    P5p5_OUT,   // + 5.5V - 4
    HV,         // +600V  - 5
    P5p5_BC,    // + 5.5V - 6  BC power monitoring line
    P22_IN,     // + 22V  - 7
    P5p5_IN     // + 5.5V - 8
};

enum PSU_STATUS {
    DOWN = 0,
    UP,
    RAMPUP,
    RAMPDOWN,
    TUNEUP,
    TUNEDOWN
};

enum TSENS_MODULE {
    LOCAL = 0,
    HB
};

enum ALARM_CODE {
    NONE = 0,
    OVERVOLTAGE,
    OVERCURRENT,
    UNDERVOLTAGE,
    CARDOVERTEMP,
    TRIP
};

typedef struct pwline {

    BYTE config;                        // feature list

    unsigned char *swport;              // switch port
    BYTE swpin;                         // switch pin
    BYTE vmuxch;                        // mux channel (voltage)
    BYTE imuxch;                        // mux channel (current)
    UINT16 vcoeff;                      // ADC conversion coefficient (voltage)
    UINT16 icoeff;                      // ADC conversion coefficient (current)
    
    enum PSU_STATUS status;
    enum ALARM_CODE alarm;

    UINT16 Vnorm;
    UINT16 Vmin;
    UINT16 Vmax;
    UINT16 Imax;
    UINT16 Iabsmax;

} pwline_t;

typedef struct _HV {

    BYTE config;                        // feature list
    
    unsigned char *swport;              // switch port
    BYTE swpin;                         // switch pin
    BYTE vmuxch;                        // mux channel (voltage)
    BYTE imuxch;                        // mux channel (current)
    UINT16 vcoeff;                      // ADC conversion coefficient
    UINT16 icoeff;                      // ADC conversion coefficient (current)

    enum PSU_STATUS status;
    enum ALARM_CODE alarm;

    UINT16 Vset;                        // current HV level
    UINT16 Vbound;                      // upper limit for HV level

    UINT16 Vmin;
    UINT16 Vmax;
    UINT16 Vabsmax;
    UINT16 Imax;
    UINT16 Iabsmax;

    UINT16 Vrampup;
    UINT16 Vrampdown;

} hv_t;

typedef struct _TSENS {

    BYTE vmuxch;                        // mux channel
    UINT16 vcoeff;                      // ADC conversion coefficient
    UINT16 offset;                      // ADC offset

    UINT16 Tmax;

} tsens_t;

typedef struct _PSU {

   pwline_t line[MAX_PWL_ID+1]; // 4xDC-DC, 1x5p5_OUT, 1x22v_IN, 1x5p5_IN + HV (not used) + BC current monitor
   tsens_t temps[MAX_TMP_ID+1]; // 2xtemperature sensors
   hv_t hv;

} psu_t;

void psu_init(void);
BYTE ps_poweron(enum PSU_MODULE mod);
BYTE ps_poweroff(enum PSU_MODULE mod, UINT force);
BYTE ps_getconfig(enum PSU_MODULE mod);

BOOL ps_memrd_vthr(enum PSU_MODULE mod, UINT16 *Vmin, UINT16 *Vmax);
BOOL ps_memrd_cthr(enum PSU_MODULE mod, UINT16 *Imax);
BOOL ps_memrd_tthr(enum TSENS_MODULE mod, UINT16 *Tmax);
void ps_memwr_vthr(enum PSU_MODULE mod, UINT16 Vmin, UINT16 Vmax);
void ps_memwr_cthr(enum PSU_MODULE mod, UINT16 Imax);
void ps_memwr_tthr(enum TSENS_MODULE mod, UINT16 Tmax);

UINT16 ps_gettemp(enum TSENS_MODULE mod);
UINT16 ps_getvoltage(enum PSU_MODULE mod);
UINT16 ps_getcurrent(enum PSU_MODULE mod);

BYTE ps_setvoltage_thr(enum PSU_MODULE mod, UINT16 Vmin, UINT16 Vmax);
BYTE ps_setcurrent_thr(enum PSU_MODULE mod, UINT16 Imax);
BYTE ps_settemp_thr(enum TSENS_MODULE mod, UINT16 Tmax);

void ps_sethv_vbound(UINT16 V);
UINT16 ps_gethv_vbound(void);
UINT16 ps_gethv_vset(void);
void ps_sethv_voltage(UINT16 V);
void ps_sethv_ramprate(UINT16 rate);
void ps_sethv_on(void);
void ps_sethv_off(void);
UINT16 ps_gethv_ramprate(void);
UINT16 ps_gethv_vabsmax(void);

UINT16 ps_getcurrent_absmax(enum PSU_MODULE mod);
UINT16 ps_getvoltage_minthr(enum PSU_MODULE mod);
UINT16 ps_getvoltage_maxthr(enum PSU_MODULE mod);
UINT16 ps_getcurrent_maxthr(enum PSU_MODULE mod);
UINT16 ps_gettemp_thr(enum TSENS_MODULE mod);

void ps_setstatus(enum PSU_MODULE mod, enum PSU_STATUS status);
enum PSU_STATUS ps_getstatus(enum PSU_MODULE mod);
enum ALARM_CODE ps_getalarm(enum PSU_MODULE mod);
void ps_setalarm(enum PSU_MODULE mod, enum ALARM_CODE acode);

enum ALARM_CODE ps_linealarm(enum PSU_MODULE mod);
enum ALARM_CODE ps_tempalarm(void);

#endif

// EOF

