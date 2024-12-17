#include "draw.h"
#include "gfx.h"

int main() {
  DSPC = MODE3 | BG2;
  int t = 0;

  while (1) {
    VBLANK();

    for (int x = 0; x < 240; x++) {
      for (int y = 0; y < 160; y++) {
        int color = (x ^ y);

        plotPixel(x, y, RGB(color, color, color ^ t));
      }
    };

    t++;
  }

  return 0;
}
