#include "graphics/draw.h"
#include "graphics/video.h"
#include "input/in.h"

int main() {
  DSPC = MODE3 | BG2;

  Coordinate start = {50, 60};
  int rWidth = 40;
  int rHeight = 80;

  unsigned short colors[] = {0x001F, 0x03E0, 0x7C00, 0x7FFF, 0x03FF, 0x7FE0,
                             0x7C1F, 0x4631, 0x1CE7, 0x021F, 0x03E7, 0x7C63,
                             0x7FFF, 0x1F1F, 0x03A0, 0x7C00};

  int cI = 0;
  unsigned short COLOR = colors[cI];
  while (1) {
    updateKeys();
    VBLANK();
    drawRect(start, rWidth, rHeight, 0x0000);

    if (keyDown(A)) {
      start.x = 50;
      start.y = 60;
      rWidth = 20;
      rHeight = 20;
    }

    if (keyDown(U)) {
      start.y--;
    }

    if (keyDown(D)) {
      start.y++;
    }
    if (keyDown(L)) {
      start.x--;
    }
    if (keyDown(R)) {
      start.x++;
    }
    if (keyDown(SL)) {
      rWidth = 20;
      rHeight = 20;
    } else if (keyDown(LT)) {
      rWidth++;
    } else if (keyDown(RT)) {
      rHeight++;
    }

    // Change color if START button is pressed
    if (keyDown(ST)) {
      if (cI >= 14) {
        cI = 0;
      } else {
        cI++;
      }
      COLOR = colors[cI];
    }
    drawRect(start, rWidth, rHeight, COLOR);
  }
  return 0;
}
