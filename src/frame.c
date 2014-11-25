#include "frame.h"
#include "utils.h"

#include <stdio.h>      // for sprintf
#include <string.h>     // for memset, strncpy

void frame_init(unsigned char *f) {
   f[0] = '\0';
}

UINT frame_docrc(unsigned char *f) {

   int i;
   UINT crc = 0;

   for (i = 0; f[i] != KW_END; i++)
      crc ^= f[i];

   return crc;
}

UINT frame_getidb(unsigned char *f) {
   char idb[4];
   UINT IDB = 0;
   idb[0] = f[1];
   idb[1] = f[2];
   idb[2] = f[3];
   idb[4] = '\0';
   IDB = htoi(idb);
   return (IDB);
}

unsigned char frame_getseq(unsigned char *f) {

   if ((f[7] == SEQNUM_DELIMITER) && (f[6] >= 0x21) && (f[6] <= 0x7E))
      return (f[6]);
   else
      return (0);
}

void frame_getdata(unsigned char *f, char *d) {

   UINT fsize = strlen((char *)f);

   if ((f[7] == SEQNUM_DELIMITER) && (f[6] >= 0x21) && (f[6] <= 0x7E)) {

      strncpy(d, (const char *) f + 8, fsize - 11);
      d[fsize - 11] = '\0';

   } else {

      strncpy(d, (const char *) f + 6, fsize - 9);
      d[fsize - 9] = '\0';
   }
}

UINT frame_getcrc(unsigned char *f) {
   char crc[3];
   UINT CRC = 0;
   UINT fsize = strlen((char *)f);

   crc[0] = f[fsize - 2];
   crc[1] = f[fsize - 1];
   crc[2] = '\0';
   CRC = htoi(crc);
   return (CRC);
}

// EOF

