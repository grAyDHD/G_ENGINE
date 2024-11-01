#include "engine.h"

extern void fillScreen(u32 clr);
#define dblClr(color) ((color) | (color << 16))

int main() {
  DSPC = MODE3 | BG2;

  u32 color = RGB(0, 2, 20);

  int r = 0, g = 2, b = 20;
  int dr = 1, dg = 1, db = 1;
  while (1) {
    for (volatile int x = 0; x < 100000; x++)
      ;

    r = (r + dr) % 32;
    g = (g + dg) % 32;
    b = (b + db) % 32;
    color = RGB(r, g, b);
    //    color = (color | color << 16);
    color = dblClr(color);
    VBLANK();
    fillScreen(color);
  }

  return 0;
}
