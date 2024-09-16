#include "draw.h"
#include "gfx.h"

void pPixel(int x, int y, u16 clr) { ((u16 *)VRAM)[y * SW + x] = clr; }
int main() {
  DSPC = (MODE3 | BG2);
  unsigned short green = 0x03E0;
  pPixel(12, 24, green);
  return 0;
}
