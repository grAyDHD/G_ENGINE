#include "engine.h"
// #define BG2_X_ROTATION *(volatile u16 *)0x4000022 // Skewing/rotation in X
// #define BG2_Y_ROTATION *(volatile u16 *)0x4000024 // Skewing/rotation in Y

#define BG2_X_SCALING *(volatile u16 *)0x4000020 // Scaling for X axis
#define BG2_Y_SCALING *(volatile u16 *)0x4000026 // Scaling for Y axis

#define BG2_X_SCROLLING *(volatile u32 *)0x4000028
#define BG2_Y_SCROLLING *(volatile u32 *)0x400002C

void m5_plotPixel(int x, int y, u16 clr) { ((u16 *)VRAM)[y * 160 + x] = clr; }

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
