#include "../build/lil_guy.h"
#include "engine.h"

/*
 * extern void SpriteFrame(int x, int y, int frame, int row, int width, int
 * height, SpriteFrame x/y coordinate, frame#, row# ,dimx, dimymy m
 */
extern void SpriteFrame(int x, int y, int frame, const void *image);

int main() {
  DSPC = MODE3 | BG2;

  SpriteFrame(40, 40, 0, lil_guyBitmap);
  while (1) {
    VBLANK();
  }
  return 0;
}
