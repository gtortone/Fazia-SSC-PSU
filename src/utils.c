#include "utils.h"
#include "delays.h"

//#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include <EEP.h>

void __delay_s(UINT32 sec) {

    UINT32 val = sec * 1000;
    UINT32 i;

    for(i=0;i<val;i++)
        Delay10KTCYx(2);       // sleep 1 msec
}

unsigned int htoi(const char *ptr) {

   unsigned int value = 0;
   char ch = *ptr;

   while (ch == ' ' || ch == '\t')
      ch = *(++ptr);

   for (;;) {

      if (ch >= '0' && ch <= '9')
         value = (value << 4) + (ch - '0');
      else if (ch >= 'A' && ch <= 'F')
         value = (value << 4) + (ch - 'A' + 10);
      else if (ch >= 'a' && ch <= 'f')
         value = (value << 4) + (ch - 'a' + 10);
      else
         return value;
      ch = *(++ptr);
   }
}

unsigned char ReadEEPROM(unsigned char address) {

   return(Read_b_eep (address));

//   EECON1 = 0; //ensure CFGS=0 and EEPGD=0
//   EEADR = address;
//   EECON1bits.RD = 1;
//   return (EEDATA);
}

void WriteEEPROM(unsigned char address, unsigned char data) {

   Write_b_eep (address, data);
   Busy_eep ();

//   char SaveInt;
//
//   SaveInt = INTCON; //save interrupt status
//   EECON1 = 0; //ensure CFGS=0 and EEPGD=0
//   EECON1bits.WREN = 1; //enable write to EEPROM
//   EEADR = address; //setup Address
//   EEDATA = data; //and data
//   INTCONbits.GIE = 0; //No interrupts
//   EECON2 = 0x55; //required sequence #1
//   EECON2 = 0xaa; //#2
//   EECON1bits.WR = 1; //#3 = actual write
//   INTCON = SaveInt; //restore interrupts
//   while (!PIR2bits.EEIF); //wait until finished
//   EECON1bits.WREN = 0; //disable write to EEPROM
}

// EOF
