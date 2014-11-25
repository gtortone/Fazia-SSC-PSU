#include <usart.h>
#include <timers.h>

#include "setup.h"
#include "board.h"
#include "spi-dev.h"
#include "comb-dev.h"
#include "psu.h"
#include "uarterr.h"

#define BAUDRATE  115200
#define _SPBRG ((FOSC/BAUDRATE)/16)-1

void ucsetup(void) {

   // configure USART
   TXSTA = 0; // Reset USART registers to POR state
   RCSTA = 0;
   TRISCbits.RC6 = 1;
   TRISCbits.RC7 = 1;
   TXSTAbits.BRGH = 1;
   SPBRG = _SPBRG; // Write baudrate to SPBRG1
   SPBRGH = _SPBRG >> 8; // For 16-bit baud rate generation
   TXSTAbits.SYNC = 0;
   TXSTAbits.TXEN = 1; // Enable transmitter
   RCSTAbits.SPEN = 1; // Enable receiver
   RCSTAbits.CREN = 1;

   // USART interrupts
   IPR1bits.RCIP = 1; // USART Rx on high priority interrupt
   PIE1bits.RCIE = 1; // Enable Rx interrupts

   //Timer1 Registers Prescaler= 8 - TMR1 Preset = 0 - Freq = 30 Hz
   T1CONbits.T1CKPS1 = 1; // bits 5-4  Prescaler Rate Select bits
   T1CONbits.T1CKPS0 = 1; // bit 4
   T1CONbits.T1OSCEN = 0; // bit 3 Timer1 Oscillator Enable Control bit 1 = on
   T1CONbits.T1SYNC = 1; // bit 2 Timer1 External Clock Input Synchronization Control bit...1 = Do not synchronize external clock input
   T1CONbits.TMR1CS = 0; // bit 1 Timer1 Clock Source Select bit...0 = Internal clock (FOSC/4)
   T1CONbits.TMR1ON = 1; // bit 0 enables timer
   TMR1H = 3; // preset for timer1 MSB register
   TMR1L = 120; // preset for timer1 LSB register

   PIR1bits.TMR1IF = 0; // clear timer1 interupt flag TMR1IF
   IPR1bits.TMR1IP = 0; // Timer1 on low priority interrupt
   PIE1bits.TMR1IE = 1; // enable Timer1 interrupts

   // internal ADC
   TRISAbits.TRISA0 = 1;// AN0

   // ADC AD7694
   TRISCbits.RC2 = 0;   // CS
   TRISCbits.RC3 = 0;   // SCK
   TRISCbits.RC4 = 1;   // SDI

   // DAC
   TRISBbits.RB0 = 0;   // CS
   TRISBbits.RB1 = 0;   // SCK
   TRISBbits.RB2 = 0;   // SDO

   // MUX
   TRISBbits.RB3 = 0;   // MUX_H
   TRISBbits.RB4 = 0;   // MUX_L
   TRISBbits.RB5 = 0;   // A0
   TRISBbits.RB6 = 0;   // A1
   TRISBbits.RB7 = 0;   // A2

   // PSU modules
   LATAbits.LATA4 = 0;     // +5V enable
   TRISAbits.RA4 = 0;
   LATAbits.LATA2 = 0;     // -5V enable
   TRISAbits.RA2 = 0;   
   LATCbits.LATC1 = 0;     // +12V enable
   TRISCbits.RC1 = 0;
   LATAbits.LATA1 = 0;     // -12V enable
   TRISAbits.RA1 = 0;
   LATAbits.LATA5 = 0;     // +5.5V enable
   TRISAbits.RA5 = 0;
   LATCbits.LATC0 = 0;     // HV enable
   TRISCbits.RC0 = 0;

   ANSEL = 1;              // _only_ AN0 as analog input
   ANSELH = 0;             // other analog pins configured as digital I/O

   // internal ADC
   ADCON0bits.CHS = 0;     // select channel 0 (AN0)
   ADCON1bits.VCFG0 = 1;   // pos reference +Vref
   ADCON1bits.VCFG1 = 0;   // neg reference Vss
   ADCON2bits.ADCS = 6;    // Tad = Fosc/64
   ADCON2bits.ACQT = 7;    // 20 Tad (acquisition time)
   ADCON2bits.ADFM = 1;    /// risultato giustificato a dx
   ADCON0bits.ADON = 1;       // switch on the adc module

   RCONbits.IPEN = 1; // enable interrupt priority levels
   INTCONbits.GIE = 1; // enable all priority interrupts
   INTCONbits.PEIE = 1;
}

void memsetup(void) {

   extern UartErrLog uerrlog;

   psu_init();
   adc_init();
   dac_init();
   mux_init();

   // reset UART error log
   uerrlog.uart_ferr = 0;
   uerrlog.uart_oerr = 0;

   ps_sethv_voltage(0);    // safe mode for HV
   ps_poweroff(HV,FORCE);
   ps_poweroff(P5, FORCE);        // and other power lines
   ps_poweroff(M5, FORCE);
   ps_poweroff(M8, FORCE);
   ps_poweroff(P12, FORCE);
   ps_poweroff(P5p5_OUT, FORCE);
      
#ifdef LOOPBACK_MODE
      ps_poweron(P5);
      ps_poweron(M5);
      ps_poweron(M8);
      ps_poweron(P12);
      ps_poweron(P5p5_OUT);
#endif
}

// EOF
