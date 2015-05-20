#ifndef _UTILS_H
#define _UTILS_H

#include "Generic.h"

#define MULTIPLIER 100

void __delay_s(UINT32 sec);
unsigned int htoi(const char *ptr);

unsigned char ReadEEPROM(unsigned char address);
void WriteEEPROM(unsigned char address, unsigned char data);

#endif

// EOF

