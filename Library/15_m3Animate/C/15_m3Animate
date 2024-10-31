#include "../build/lil_guy.h"
#include "engine.h"

extern void SpriteFrame(int x, int y, int frame, const void *image,
                        int frameCount);

int main() {
  DSPC = MODE3 | BG2;

  int pX = 0, pY = 0;
  while (1) {
    VBLANK();
    for (volatile int x = 0; x < 2000; x++)
      ;
    SpriteFrame(pX, pY, 0, lil_guyBitmap, 4);
    pX += 16;

    for (volatile int x = 0; x < 2000; x++)
      ;
    SpriteFrame(pX, pY, 1, lil_guyBitmap, 4);
    pX += 16;

    for (volatile int x = 0; x < 2000; x++)
      ;
    SpriteFrame(pX, pY, 2, lil_guyBitmap, 4);
    pX += 16;

    for (volatile int x = 0; x < 2000; x++)
      ;
    SpriteFrame(pX, pY, 3, lil_guyBitmap, 4);
    pX += 16;
  }
  return 0;
}
