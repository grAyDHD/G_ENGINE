#include "graphics/video.h"

void VBLANK() {
  while (VCOUNT >= 160)
    ;
  while (VCOUNT < 160)
    ;
}
