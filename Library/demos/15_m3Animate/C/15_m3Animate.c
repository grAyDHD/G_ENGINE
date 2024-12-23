#include "../build/lil_guy.h"
#include "core/timer.h"
#include "graphics/draw.h"
#include "graphics/video.h"

void animation(int x, int y, int frame, const void *image);
// handles 4 frames
extern void SpriteFrame16(int x, int y, int frame, const void *image,
                          int frameCount);

int main() {
  DSPC = MODE3 | BG2;

  int animationFrame = 0;
  while (1) {
    VBLANK();
    SpriteFrame16(32, 32, animationFrame % 4, lil_guyBitmap, 3);
    animationFrame++;
    simpleWait(100);
  }

  return 0;
}

void animation(int x, int y, int frame, const void *image) {
  for (int w = 0; w < 15; w++) {
    for (int h = 0; h < 10; h++) {
      if (w > 0)
        SpriteFrame(x + (w * 16), y + (h * 16), frame, image, 4);
      if (w == 0)
        SpriteFrame(0, y + (h * 16), frame, image, 4);
    }
  }
}

/*
VBLANK();
simpleWait(100);
animation(0, 0, 0, lil_guyBitmap);

simpleWait(100);
animation(0, 0, 1, lil_guyBitmap);

simpleWait(100);
animation(0, 0, 2, lil_guyBitmap);

simpleWait(100);
animation(0, 0, 3, lil_guyBitmap);
*/
