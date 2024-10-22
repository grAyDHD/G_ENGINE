#include "gfx.h"
#include "typedefs.h"

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
