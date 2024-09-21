#include "gfx.h"

void waitVBLANK() {
  while (VCOUNT >= 160)
    ; // vcount range from 0 to 159, for 160 lines of gba 240x160
  while (VCOUNT < 160)
    ; //
}
