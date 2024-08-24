#include <audio.h>
#include <draw.h>
#include <gfx.h>
#include <input.h>

#define GW 30 // GRID WIDTH = SW/8
#define GH 20 // GRID HEIGHT = SH/8

void initializeScreen(unsigned short *vmem) {
  DSPC = MODE3 | BG2;
  unsigned short black = COLOR(1, 5, 3);
  for (int x = 0; x < SW; x++) {
    for (int y = 0; y < SH; y++) {
      plotPixel(x, y, black, vmem);
    }
  }
}

int main() {
  initializeScreen(VRAM);

  return 0;
}
