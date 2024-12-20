#include "gfx/gfx.h"

void VBLANK() {
  while (VCOUNT >= 160)
    ;
  while (VCOUNT < 160)
    ;
}
