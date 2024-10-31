#include "../build/lil_guy.h"
#include "engine.h"

extern void SpriteFrame(int x, int y, int frame, const void *image,
                        int frameCount);

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

int main() {
  DSPC = MODE3 | BG2;

  while (1) {
    VBLANK();
    for (volatile int z = 0; z < 30000; z++)
      ;
    animation(0, 0, 0, lil_guyBitmap);

    for (volatile int z = 0; z < 30000; z++)
      ;
    animation(0, 0, 1, lil_guyBitmap);
    for (volatile int z = 0; z < 30000; z++)
      ;
    animation(0, 0, 2, lil_guyBitmap);

    for (volatile int z = 0; z < 30000; z++)
      ;
    animation(0, 0, 3, lil_guyBitmap);
  }

  return 0;
}
