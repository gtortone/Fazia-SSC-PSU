#ifndef _UTILS_H
#define _UTILS_H

#define MULTIPLIER 100

unsigned int htoi(const char *ptr);

unsigned char ReadEEPROM(unsigned char address);
void WriteEEPROM(unsigned char address, unsigned char data);

#endif

// EOF

