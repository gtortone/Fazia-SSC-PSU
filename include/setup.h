#ifndef _SETUP_H
#define _SETUP_H

#define MUX_EN_H        LATBbits.LATB3
#define MUX_EN_L        LATBbits.LATB4
#define MUX_A0          LATBbits.LATB5
#define MUX_A1          LATBbits.LATB6
#define MUX_A2          LATBbits.LATB7

#define EN_P12          LATCbits.LATC1

//#define LOOPBACK_MODE

void ucsetup(void);
void memsetup(void);

#endif

// EOF

