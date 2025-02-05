#include "graphics/draw.h"
#include "graphics/video.h"

int main() {
  DSPC = MODE3 | BG2;

  unsigned short green = 0x03E0;
  for (int x = 0; x < 240; x++) {
    for (int y = 0; y < 160; y++) {
      plotPixel(x, y, green + x);
    }
  }
  return 0;
}
