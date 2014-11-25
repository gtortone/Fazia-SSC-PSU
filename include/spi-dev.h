#ifndef _SPI_DEV_H
#define _SPI_DEV_H

#include <Generic.h>
#include "spi-sw.h"

typedef struct {
    SPI_param SPIp;
} adcdev_t;

typedef struct {
    SPI_param SPIp;
} dacdev_t;

void adc_init(void);
UINT16 adc_read(void);

void dac_init(void);
void dac_write(UINT16 word);

#endif

// EOF

