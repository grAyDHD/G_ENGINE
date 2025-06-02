#include "graphics/draw.h"
#include "graphics/video.h"
#include "graphics/m3Text.h"

int main() {
  DSPC = MODE3 | BG2;
  int bgColor = dblClr(RGB(17, 13, 28));
  fillScreen(bgColor);

  gprintf(20, 25, "ABC %x abc {1, 2, 3, 0 }", 0xDEADBEAF);
  while (1) {}
  return 0;
}
