#include "gfx/draw.h"
#include "gfx/gfx.h"

int main() {
  DSPC = (MODE3 | BG2);
  u16 clr = RGB(13, 19, 28);
  plotPixel(12, 24, clr);
  return 0;
}
