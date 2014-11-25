#ifndef _SW_SPI_H_
#define _SW_SPI_H_

#include <Generic.h>

/*
PIN structure:
	*R can point to PORT or LAT register depending on pin direction
	(PORT : input) (LAT : output)
	>>REMEMBER<< to set the TRIS correct value: (0 : out) (1 : in)
	>>this settings are made in SPI_init() function<<

example:
	const SPI_pin V_SDI = { &PORTB, &TRISB, 3 }; //PIN B3 is set for reading  input,

*/

typedef struct {
    volatile unsigned char *port;
    volatile unsigned char *tris;
    BYTE npin;
} SPI_pin;

typedef struct {
    SPI_pin SDI;
    SPI_pin SDO;
    SPI_pin SCK;
    SPI_pin SS;
    UINT8 bitsize;
} SPI_param;

void SPI_dly();
void SPI_bw(SPI_pin pin, BOOL s);
BOOL SPI_br(SPI_pin pin);
void SPI_init(SPI_param *p);
void SPI_tx(SPI_param *p, UINT16 data);
UINT16 SPI_rx(SPI_param *p);

#endif

// EOF

