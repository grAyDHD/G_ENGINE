#include "engine.h"

int main() {
  // initialize display
  DSPC = MODE4 | BG2;

  // loop16x16 to set all palette colors
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      setPaletteColor((x * 16 + y), RGB(x * y, 0, x * y));
    }
  }

  // loop 240x160 to plot all pixels on page2
  // all values at given x same clrid,
  for (int x = 0; x < 240; x++) {
    for (int y = 0; y < 160; y++) {
      m4_plotPixel(x, y, x);
    }
  }

  pageFlip();

  for (int x = 0; x < 240; x++) {
    for (int y = 0; y < 160; y++) {
      m4_plotPixel(240 - x, y, x);
    }
  }

  pageFlip();

  while (1) {
    VBLANK();
    for (volatile int i = 0; i <= 1000000; i++)
      ;

    pageFlip();
  }
  return 0;
}
