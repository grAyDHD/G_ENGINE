#include "engine.h"
#include "gfx.h"

#define PALRAM ((volatile u16 *)0x5000000)

void setPaletteColor(u8 index, u16 color) { PALRAM[index] = color; }

#define VRAM_PAGE1 ((u16 *)0x6000000)
#define VRAM_PAGE2 ((u16 *)0x600A000)

#define PAGE_OFFSET 0xA000
#define FLIP 0x0010
u16 *bufferPage = VRAM_PAGE2;

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
  DSPC = MODE4 | BG2;
  setPaletteColor(0, RGB(0, 0, 0));   // Palette index 0: Red
  setPaletteColor(1, RGB(0, 31, 0));  // Palette index 1: Green
  setPaletteColor(2, RGB(0, 0, 31));  // Palette index 2: Blue
  setPaletteColor(3, RGB(31, 31, 0)); // Palette index 3: Yellow
  setPaletteColor(4, RGB(31, 0, 31)); // Palette index 4: Magenta
  setPaletteColor(5, RGB(0, 31, 31)); // Palette index 5: Cyan

  m4_plotPixel(50, 50, 0);
  m4_plotPixel(100, 80, 1);
  m4_plotPixel(150, 120, 2);

  pageFlip();

  m4_plotPixel(60, 60, 3);
  m4_plotPixel(110, 90, 4);
  m4_plotPixel(160, 130, 5);

  pageFlip();

  while (1) {
    VBLANK();
    for (volatile int i = 0; i <= 100000; i++)
      ;

    pageFlip();
  }
  return 0;
}
