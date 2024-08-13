#include "draw.h"
#include "gfx.h"

int main() {
  DSPC = (MODE3 | BG2);
  unsigned short green = 0x03E0;
  plotPixel(12, 24, green, VRAM);
  return 0;
}
