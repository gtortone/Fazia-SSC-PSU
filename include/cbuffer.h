#ifndef _CBUFFER_H
#define _CBUFFER_H

#include <Generic.h>

#define BUFFER_SIZE         270

#define BUFFER_OK           0x00
#define BUFFER_FULL         0x0F
#define BUFFER_EMPTY        0x0E

// Circular buffer struct

typedef struct {
    UINT16 size;
    UINT16 rdp; // READ pointer
    UINT16 wrp; // WRITE pointer
    unsigned char data[BUFFER_SIZE]; // vector of elements
} CBuffer;

void cbuffer_init(CBuffer *cb);

#define cbuffer_isfull(cb)      ((((*cb).wrp + 1) % (*cb).size) == (*cb).rdp)
#define cbuffer_isempty(cb)     ((*cb).wrp == (*cb).rdp)

BYTE cbuffer_write(CBuffer *cb, unsigned char ch);
BYTE cbuffer_read(CBuffer *cb, unsigned char *ch);

UINT cbuffer_getframe_length(CBuffer *cb, UINT *flen, UINT *foffset);

unsigned char *cbuffer_dumpdata(CBuffer *cb);

#endif

// EOF
