#include "spi-dev.h"
#include "board.h"

static adcdev_t adc;
static dacdev_t dac;

void adc_init(void) {

   //ADC (AD7694) init
   adc.SPIp.SDI.port = &PORTC;
   adc.SPIp.SDI.tris = &TRISC;
   adc.SPIp.SDI.npin = 4;
   adc.SPIp.SCK.port = &PORTC;
   adc.SPIp.SCK.tris = &TRISC;
   adc.SPIp.SCK.npin = 3;
   adc.SPIp.SS.port = &LATC;
   adc.SPIp.SS.tris = &TRISC;
   adc.SPIp.SS.npin = 2;
   adc.SPIp.bitsize = 16;
   SPI_init(&(adc.SPIp));
}

UINT16 adc_read(void) {

   return(SPI_rx(&(adc.SPIp)));
}

void dac_init(void) {

   //DAC init
   dac.SPIp.SDO.port = &PORTB;
   dac.SPIp.SDO.tris = &TRISB;
   dac.SPIp.SDO.npin = 2;
   dac.SPIp.SCK.port = &PORTB;
   dac.SPIp.SCK.tris = &TRISB;
   dac.SPIp.SCK.npin = 1;
   dac.SPIp.SS.port = &LATB;
   dac.SPIp.SS.tris = &TRISB;
   dac.SPIp.SS.npin = 0;
   dac.SPIp.bitsize = 16;
   SPI_init(&(dac.SPIp));
}

void dac_write(UINT16 word) {

   SPI_tx(&(dac.SPIp), word);
}

// EOF

