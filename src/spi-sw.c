#include "spi-sw.h"
#include "board.h"

/*************************************************************************
function:
   void SPI_dly()

description:
   waits 5 us

parameters:
   none

example:
   SPI_BW(SCK,1);
   SPI_dly();
   SPI_BW(SCK,0);
   SPI_dly();

 *************************************************************************/
void SPI_dly() {
   __delay_us(25);
}

/*************************************************************************
function:
   inline void SPI_bw(SPI_pin *pin, BOOL s)

description:
   Sets or Clear SDO, SCK, SS output pins

parameters: *pin

 *pin: pointer to _SPI_pin struct, it containes all the tris,port,bit register info
 *SS[n] in LMK02000.h stores all the 'Slave Select' pin of all the SPI stream used
example:


   SPI_bw( SS[n], 0 );	//clear Slave Select, start SPI
   SPI_bw( SS[n], 1 );	//sets Slave Select, stop SPI
 *************************************************************************/
void SPI_bw(SPI_pin pin, BOOL s) {

   if (s == 1)
      (*(pin.port) |= (1 << pin.npin));
   else
      (*(pin.port) &= (~(1 << pin.npin)));
}

/*************************************************************************
function:
   inline BOOL SPI_br(_SPI_pin *pin)

description:
   reads SDI bit during SPI_BB_rx

parameters: *pin

 *pin: pointer to SPI_pin struct, it containes all the tris,port,bit register info

example:
   RData |= ( SPI_BR( SDI ) << i );
 *************************************************************************/
BOOL SPI_br(SPI_pin pin) {
   return (*(pin.port) >> pin.npin) & 1;
}

/*************************************************************************
function:
   void SPI_init(SPI_param *p)

description:
   initialize TRIS reigister of all the SPI streams

parameters:
   p: SPI parameters

example:
   SPI_init(PLL1->SPI_param);
 *************************************************************************/
void SPI_init(SPI_param *p) {

   SPI_bw(p->SS, 1); //output high on each SS pin
   SPI_bw(p->SDO, 0);
   SPI_bw(p->SCK, 1);
   SPI_bw(p->SDI, 1);
}

/*************************************************************************
function:
   void SPI_tx(SPI_param *p, UINT16 data)

description:
   sends 'data' variable size values on the  SPI stream

parameters:
   p:      SPI parameters
   data:	32bit value, it size depends on the different streams

example:
   SPI_BB_tx(FPGA_A->SPI_param, Address);
   SPI_BB_tx(FPGA_D->SPI_param, Data);
 *************************************************************************/
void SPI_tx(SPI_param *p, UINT16 data) {

   int i;
   BOOL b;

   SPI_bw(p->SS, 0); //clear Slave Select, start SPI

   __delay_us(100);  // for DAC

   for (i = (p->bitsize) - 1; i >= 0; i--) {
      b = (BOOL) (data >> i) & 1; //data bit

      SPI_bw(p->SDO, b); //data out
      SPI_dly();
      SPI_bw(p->SCK, 1); //clock set
      SPI_dly();
      SPI_bw(p->SCK, 0); //clock clear
   }

   __delay_us(100);
   SPI_bw(p->SS, 1); //set Slave Select, end SPI
}

/*************************************************************************
function:
   UINT32 SPI_rx(UINT8 n)

description:
   receives 'data' variable size values from the SPI stream
   returns received data in a 32bit variable (real size depends on the selected stream)

parameters:
   p:      SPI parameters

example:
   SPI_BB_tx(FPGA_A->SPI_param, Read_Address);
   Data = SPI_BB_rx(FPGA_D->SPI_param);
 *************************************************************************/
UINT16 SPI_rx(SPI_param *p) {

   int i;
   UINT16 tmp = 0;
   UINT16 RData = 0;

   SPI_bw(p->SS, 1);
   __delay_us(10);
   SPI_bw(p->SS, 0); //clear Slave Select, start SPI

   for (i = (p->bitsize - 1); i >= 0; i--) {

      SPI_bw(p->SCK, 1); //clock set
      __delay_us(1);
      tmp = SPI_br(p->SDI);
      RData |= (tmp << i); //data in
      SPI_bw(p->SCK, 0); //clock clear
   }

   return RData;
}

// EOF

