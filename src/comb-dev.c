#include <p18cxxx.h>

#include "board.h"
#include "setup.h"
#include "comb-dev.h"

void mux_init(void) {

   MUX_EN_H = 0;
   MUX_EN_L = 0;

   MUX_A0 = 0;
   MUX_A1 = 0;
   MUX_A2 = 0;
}

void mux_select(BYTE channel) {

   MUX_EN_H = bitget(channel, 3);
   MUX_EN_L = ~MUX_EN_H;
   MUX_A0 = bitget(channel, 0);
   MUX_A1 = bitget(channel, 1);
   MUX_A2 = bitget(channel, 2);

   //__delay_ms(5); // time for output stability
}

// EOF

