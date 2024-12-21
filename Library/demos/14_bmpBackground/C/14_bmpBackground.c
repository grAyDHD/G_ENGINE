#include "../build/MockSnek.h"
#include "core/engine.h"
#include "graphics/draw.h"
#include "graphics/video.h"

int main() {
  DSPC = MODE3 | BG2;
  memCopy32(VRAM, MockSnekBitmap, MockSnekBitmapLen / sizeof(u32));

  // m3_Background(MockSnekBitmap);
  while (1) {
    VBLANK();
  }

  return 0;
}
