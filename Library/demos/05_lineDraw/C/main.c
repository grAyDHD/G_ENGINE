#include "engine.h"

int main() {
  DSPC = (MODE3 | BG2);

  unsigned short green = 0x03E0;

  Coordinate start = {0, 0};
  Coordinate end = {80, 120};
  drawLine(start, end, green);

  return 0;
}
