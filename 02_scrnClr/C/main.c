#include "engine.h"

extern void fillScreen(u32 clr);

int main() {
  DSPC = MODE3 | BG2;

  u16 color = COLOR(0, 2, 20);
  u32 dblClr = (color) | (color << 16);

  fillScreen(dblClr);

  return 0;
}
