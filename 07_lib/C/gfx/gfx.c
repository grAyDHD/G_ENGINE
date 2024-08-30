#include "gfx.h"
#include "typedefs.h"

void plotPixel(int x, int y, unsigned short color, unsigned short *vmem) {
  if (x < 0 || x >= SW || y < 0 || y >= SH) {
    return; // Prevent drawing outside the screen boundaries
  }
  int pixel = (y * SW) + x;
  vmem[pixel] = color;
}

void centerRect(Coordinate center, int width, int height, unsigned short color,
                unsigned short *vmem) {
  // x, y

  // corners
  // x + (height/2) is top middle
  // x + (height/2), y + (width/2)
  // x +            y -
  // y +            y +
  // y -            y -
}

void waitVBLANK() {
  while (VCOUNT >= 160)
    ; // vcount range from 0 to 159, for 160 lines of gba 240x160
  while (VCOUNT < 160)
    ; //
}
