#include "graphics/draw.h"
#include "graphics/video.h"

int main() {
  DSPC = (MODE3 | BG2);
  u16 clr = RGB(5, 7, 11);
  plotPixel(13, 69, clr);
  return 0;
}
