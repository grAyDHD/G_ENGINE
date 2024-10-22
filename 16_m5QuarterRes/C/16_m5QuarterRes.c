#include "draw.h"
#include "engine.h"
#include "gfx.h"
#include "typedefs.h"
/*
 *
BG Mode 4,5 (Bitmap based Modes)
  06000000-06009FFF  40 KBytes Frame 0 buffer (only 37.5K used in Mode 4)
  0600A000-06013FFF  40 KBytes Frame 1 buffer (only 37.5K used in Mode 4)
  06014000-06017FFF  16 KBytes OBJ Tiles

Note
Additionally to the above VRAM, the GBA also contains 1 KByte Palette RAM (at
05000000h) and 1 KByte OAM (at 07000000h) which are both used by the display
controller as well.
*
* 15bpp in mode 5, same as mode 3, but lower resolution to fill screen.

4000020h  2    W    BG2PA     BG2 Rotation/Scaling Parameter A (dx)
  4000022h  2    W    BG2PB     BG2 Rotation/Scaling Parameter B (dmx)
  4000024h  2    W    BG2PC     BG2 Rotation/Scaling Parameter C (dy)
  4000026h  2    W    BG2PD     BG2 Rotation/Scaling Parameter D (dmy)
  4000028h  4    W    BG2X      BG2 Reference Point X-Coordinate
  400002Ch  4    W    BG2Y      BG2 Reference Point Y-Coordinate

4000020h - BG2PA - BG2 Rotation/Scaling Parameter A (alias dx) (W)
4000022h - BG2PB - BG2 Rotation/Scaling Parameter B (alias dmx) (W)
4000024h - BG2PC - BG2 Rotation/Scaling Parameter C (alias dy)  (W)
4000026h - BG2PD - BG2 Rotation/Scaling Parameter D (alias dmy) (W)
  Bit   Expl.
  0-7   Fractional portion (8 bits)
  8-14  Integer portion    (7 bits)
  15    Sign               (1 bit)
See below for details.
*/
#define BG2_X_SCALING *(volatile u16 *)0x4000020  // Scaling for X axis
#define BG2_X_ROTATION *(volatile u16 *)0x4000022 // Skewing/rotation in X
#define BG2_Y_ROTATION *(volatile u16 *)0x4000024 // Skewing/rotation in Y
#define BG2_Y_SCALING *(volatile u16 *)0x4000026  // Scaling for Y axis

#define BG2_X_SCROLL_L                                                         \
  *(volatile u16 *)0x4000028 // Lower 16 bits of X reference point
#define BG2_X_SCROLL_H                                                         \
  *(volatile u16 *)0x400002A // Upper 12 bits of X reference point
#define BG2_Y_SCROLL_L                                                         \
  *(volatile u16 *)0x400002C // Lower 16 bits of Y reference point
#define BG2_Y_SCROLL_H                                                         \
  *(volatile u16 *)0x400002E // Upper 12 bits of Y reference point
void m5_plotPixel(int x, int y, u16 clr) { ((u16 *)VRAM)[y * 160 + x] = clr; }

int main() {
  DSPC = MODE5 | BG2;

  BG2_X_SCALING = 256;
  BG2_Y_SCALING = 256;

  int scroll_x = 0;
  int scroll_y = 0;

  for (int x = 0; x < 120; x++) {
    for (int y = 0; y < 80; y++) {
      m5_plotPixel(x, y, RGB(x, x + y, y));
    }
  }
  while (1) {
    VBLANK();

    for (volatile int x = 0; x < 10009; x++)
      ;

    // Scroll by increments of 10 pixels (set to 2560 since 256 = 1 pixel)
    // scroll_x += 256; // Scroll right by 1 pixels
    //     scroll_y += 256; // Scroll down by 1 pixels

    // Apply scrolling values
    BG2_X_SCROLL_L += 256;
    // BG2_X_SCROLL_L = scroll_x & 0xFFFF; // Lower 16 bits
    //     BG2_X_SCROLL_H = scroll_x >> 16;    // Upper 12 bits
    //    BG2_Y_SCROLL_L = scroll_y & 0xFFFF; // Lower 16 bits
    //    BG2_Y_SCROLL_H = scroll_y >> 16;    // Upper 12 bits
  }
  return 0;
}
