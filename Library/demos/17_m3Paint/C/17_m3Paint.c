#include "draw.h"
#include "gfx.h"

enum MODE { DRAWING = 0, COLOR_SELECT, SHAPE_SELECT };

int main() {
  DSPC = MODE3 | BG2;
  fillScreen(dblClr(RGB(3, 5, 9)));

  //  drawRect({8, 8}, 4, 32, color);

  return 0;
}
