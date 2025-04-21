#include "../includes/JudahGame.h"
#include "graphics/video.h"
#include "core/memory.h"

int main() {
  DSPC = MODE3 | BG2;
  const void *stage_images[3] = {lava_backgroundBitmap, cloud_backgroundBitmap, tree_backgroundBitmap};   
  int current_stage = 1;

  VBLANK();
  memCopy32(VRAM, stage_images[current_stage], lava_backgroundBitmapLen / sizeof(u32));

  while (1) {
    VBLANK();
  }
  return 0;
}
