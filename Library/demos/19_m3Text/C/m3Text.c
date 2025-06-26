#include "graphics/draw.h"
#include "graphics/video.h"
#include "graphics/m3Text.h"

int main() {
  DSPC = MODE3 | BG2;
  int bgColor = dblClr(RGB(17, 13, 28));
  fillScreen(bgColor);

  setTextColor(0);

  gprintf(20, 25, "ABC %x abc {1, 2, 3, 0 }", 0xDEADBEAF);
  gprintf(0, 0, "This prints, up to an extent", 0);
  while (1) {}
  return 0;
}
