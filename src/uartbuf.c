#include "uartbuf.h"
#include "frame.h"
#include "Generic.h"

#include <string.h>     // for strlen
#include <usart.h>

#ifdef DEBUG
#include <string.h>
#include <stdio.h>
#endif

#pragma udata large_udata
volatile CBuffer _Uart[3];
volatile CBuffer *Uart = _Uart;
#pragma udata

void uartbuf_init(void) {

   cbuffer_init(&Uart[SLAVE_RX]);
   cbuffer_init(&Uart[SLAVE_TX]);
   cbuffer_init(&Uart[SLAVE_HPTX]);
}

unsigned char *uartbuf_dump(UINT channel) {
   return (cbuffer_dumpdata(&Uart[channel]));
}

void uartbuf_flush(UINT channel) {

   unsigned char ch;

   if ((channel == SLAVE_TX) || (channel == SLAVE_HPTX)) {

      while (!cbuffer_isempty(&Uart[channel])) {
         cbuffer_read(&Uart[channel], &ch);
#ifdef DEBUG
         printf("(0x%03X) - ", ch);
#else
         while (BusyUSART());
         putcUSART(ch);
#endif
      }
   }
}

void uartbuf_getframe(UINT channel, unsigned char *f, UINT flen, UINT foffset) {

   UINT i;
   unsigned char ch;

   i = 0;
   while (i < (foffset - 1)) {
      cbuffer_read(&Uart[channel], &ch);
      i++;
   }

   i = 0;
   while (i < flen) {
      cbuffer_read(&Uart[channel], &ch);
      f[i++] = ch;
   }

   f[flen] = '\0';
}

void uartbuf_putframe(UINT channel, unsigned char *f) {

   UINT i;
   UINT fsize = strlen((char *)f);

   for (i = 0; i < fsize; i++)
      cbuffer_write(&Uart[channel], f[i]);

#ifdef DEBUG

   printf("\r\n\nPutFrame(");
   if (channel == SLAVE_TX)
      printf("Slave TX");
   else if (channel == SLAVE_RX)
      printf("Slave RX");
   else if (channel == SLAVE_HPTX)
      printf("Slave #HP# TX");

   printf(") for PSC: \n\r");
   printf("\tKW = 0x%X \t", frame_getkw(f));
   printf("IDB = 0x%03X \t", frame_getidb(f));
   printf("IDS = 0x%02X \t", frame_getids(f));
   printf("CMD = 0x%X \t", frame_getcmd(f));
   printf("DATA = %s \t", frame_getdata(f));
   printf("CRC = 0x%X \t", frame_getcrc(f));
   printf("fsize = %d\n\r", f->size);

#endif
}

// EOF

