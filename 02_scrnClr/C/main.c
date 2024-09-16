#include "engine.h"
#include "gfx.h"
#include "typedefs.h"

int main() {
  DSPC = MODE3 | BG2;

  u16 color = COLOR(2, 2, 20);
  for (int y = 0; y < 160; y++) {
    for (int x = 0; x < 240; x++) {
      plotPixel(x, y, color, VRAM);
    }
  }
  return 0;
}
