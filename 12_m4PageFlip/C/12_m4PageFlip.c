#include "engine.h"

#define PALETTE ((volatile u16 *)0x5000000)
#define PAGE_OFFSET 0xA000
#define FLIP 0x0010

u16 *bufferPage = ((u16 *)0x600A000);
void setPaletteColor(u8 index, u16 color) { PALETTE[index] = color; }

u16 *pageFlip() {
  bufferPage = (u16 *)((u32)bufferPage ^ PAGE_OFFSET);
  DSPC ^= FLIP;
  return bufferPage;
}

void m4_plotPixel(int x, int y, u8 clrid) {
  u16 *dst = &((u16 *)bufferPage)[(y * SW + x) / 2];
  if (x & 1) {
    *dst = (*dst & 0xFF) | (clrid << 8);
  } else {
    *dst = (*dst & ~0xFF) | clrid;
  }
}

int main() {
  // initialize display
  DSPC = MODE3 | BG2;

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

  DSPC = MODE4 | BG2;

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
