#include "core/timer.h"
#include "graphics/draw.h"
#include "graphics/video.h"

int main() {
  DSPC = MODE3 | BG2;

  u32 color = RGB(0, 2, 20);

  int r = 0, g = 2, b = 20;

  while (1) {
    simpleWait(100);

    r = (r + 1) % 32;
    g = (g + 1) % 32;
    b = (b + 1) % 32;

    color = dblClr(RGB(r, g, b));
    VBLANK();
    fillScreen(color);
  }

  return 0;
}
