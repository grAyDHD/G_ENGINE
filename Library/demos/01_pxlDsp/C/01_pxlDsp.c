#include "core/typedefs.h"

#define DSPC *(u32 *)0x4000000
#define VRAM (u16 *)0x6000000
#define SW 240
#define SH 160
#define MODE3 0x0003
#define BG2 0x0400
#define RGB(r, g, b) (((r) & 0x1F) | ((g) & 0x1F) << 5 | ((b) & 0x1F) << 10)

void plotPixel(int x, int y, u16 clr) {
  ((u16 *)VRAM)[y * SW + x] = clr;
} // mode 3
//

int main() {
  DSPC = (MODE3 | BG2);
  u16 clr = RGB(5, 7, 11);
  plotPixel(13, 69, clr);
  return 0;
}
