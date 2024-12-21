#include "graphics/draw.h"
#include "graphics/video.h"

int main() {
  DSPC = (MODE3 | BG2);
  u16 green = 0x03E0;

  Coordinate start = {20, 20};

  drawRect(start, 80, 60, green);
  return 0;
}
