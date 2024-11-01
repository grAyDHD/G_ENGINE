//
// bm_modes.c
// Combined demo of modes 3, 4 and 5
//
// (20031002 - 20060922, cearn)

// This example is a hack. Normally you aren't supposed to have a bitmap that is
// used in different modes. Modes 3 and 5 use 16 BPP images so they could, in
// theory, share graphics. However, mode 4 uses 8 BPP images.

#include "../build/MockSnek.h"
#include "gfx.h"
// extern void memCopy32(void *dst, const void *src, u32 wdn);
extern void m3_Background(const void *src);

int main() {
  DSPC = MODE3 | BG2;
  //  memCopy32(VRAM, MockSnekBitmap, MockSnekBitmapLen / sizeof(u32));

  m3_Background(MockSnekBitmap);
  while (1) {
    VBLANK();
  }

  return 0;
}
