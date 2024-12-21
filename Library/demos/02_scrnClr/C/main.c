#include "core/timer.h"
#include "core/typedefs.h"
#include "gfx/draw.h"
#include "gfx/gfx.h"

extern void fillScreen(u32 clr);
#define dblClr(color) ((color) | (color << 16))

int main() {
  DSPC = MODE3 | BG2;

  u32 color = RGB(0, 2, 20);

  int r = 0, g = 2, b = 20;
  int dr = 1, dg = 1, db = 1;
  while (1) {
    simpleWait(100);
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
