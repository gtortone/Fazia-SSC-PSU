#include "cbuffer.h"
#include "frame.h"

#include <stdlib.h>
#include <string.h>     // for memset

void cbuffer_init(CBuffer *cb) {

   cb->size = BUFFER_SIZE;
   memset(cb->data, '\0', cb->size);
   cb->rdp = cb->wrp = 0;
}

BYTE cbuffer_write(CBuffer *cb, unsigned char ch) {

   BYTE retval = BUFFER_OK;

   cb->data[cb->wrp] = ch;
   cb->wrp = (cb->wrp + 1) % cb->size;
   if (cb->wrp == cb->rdp) {
      cb->rdp = (cb->rdp + 1) % cb->size; // full, overwrite
      retval = BUFFER_FULL;
   }
   return (retval);
}

BYTE cbuffer_read(CBuffer *cb, unsigned char *ch) {

   if (cbuffer_isempty(cb))
      return BUFFER_EMPTY;

   (*ch) = cb->data[cb->rdp];
   cb->rdp = (cb->rdp + 1) % cb->size;

   return BUFFER_OK;
}

UINT cbuffer_getframe_length(CBuffer *cb, UINT *flen, UINT *foffset) {

   UINT j, len, offset;
   UINT16 lbound = cb->rdp; // buffer lowest bound
   UINT16 hbound = cb->wrp; // buffer highest bound
   UINT16 size = cb->size; // buffer size
   unsigned char ch;
   BYTE ncrc = 0;
   BOOL frame_start = FALSE;
   BOOL frame_end = FALSE;
   BOOL loop_end = FALSE;

   if (cbuffer_isempty(cb))
      return ERR_FRAME_EMPTY;

   j = lbound;
   len = 0;
   offset = 0;

   while (!loop_end && (j != hbound)) {

      ch = cb->data[j];
      j = (j + 1) % size;

      offset++;

      if (is_startkw(ch)) {

         *foffset = offset;
         len = ncrc = 0;
         len++;
         frame_start = TRUE;
         frame_end = FALSE;

         continue;
      }

      if ((ch >= 0x20) && frame_start && !frame_end) {

         len++;
         continue;
      }

      if (is_endkw(ch) && frame_start) {

         len++;
         frame_end = TRUE;
         continue;
      }

      if (frame_start && frame_end) { // two CRC bytes to read...

         len++;
         ncrc++;

         if (ncrc == 2)
            loop_end = TRUE;

         continue;
      }
   }

   if (loop_end) { // valid frame detected on buffer
      *flen = len;
      return (ERR_FRAME_NONE);
   } else
      return (ERR_FRAME_INCOMPLETE);
}

unsigned char *cbuffer_dumpdata(CBuffer *cb) {

   return (cb->data);
}

// EOF


