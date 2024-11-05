#include "draw.h"
#include "gfx.h"

int main() {
  DSPC = MODE3 | BG2;
  fillScreen(dblClr(RGB(3, 5, 9)));
  return 0;
}
