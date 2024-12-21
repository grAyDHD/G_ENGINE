#include "graphics/draw.h"
#include "graphics/video.h"

int main() {
  DSPC = MODE5 | BG2;

  BG2_X_SCALING = 256 / 4;
  BG2_Y_SCALING = 256 / 4;

  u32 scroll_x = 0;
  u32 scroll_y = 0;

  for (int x = 0; x < 120; x++) {
    for (int y = 0; y < 80; y++) {
      m5_plotPixel(x, y, RGB(x, x + y, y));
    }
  }
  while (1) {

    for (volatile int x = 0; x < 100093; x++)
      ;

    VBLANK();
    scroll_x += 256; // Scroll right by 1 pixels
    scroll_y += 256; // Scroll down by 1 pixels

    BG2_X_SCROLLING = scroll_x;
    BG2_Y_SCROLLING = scroll_y;
  }
  return 0;
}
