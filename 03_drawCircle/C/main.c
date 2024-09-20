// #include "engine.h"
#define u16 unsigned short
#define u32 unsigned long

#define COLOR(r, g, b) (((r) & 0x1F) << 10 | ((g) & 0x1F) << 5 | ((b) & 0x1F))

#define SW 240
#define SH 160

//---Display control---//
#define DSPC *(u32 *)0x4000000
//---Video buffer---
#define VRAM (u16 *)0x6000000

//---Modes---//
#define MODE3 (u16)0x400
#define BG2 (u16)0x003

void plotPixel(int x, int y, u16 clr) { ((u16 *)VRAM)[y * SW + x] = clr; }
void drawCircle(int x, int y, int radius, unsigned short color,
                unsigned short *vmem) {
  int r = radius;
  int s = 0;
  int decisionOver2 =
      1 - r; // Decision criterion divided by 2 evaluated at r=r, s=0

  while (r >= s) {
    for (volatile int z = 0; z < 100000; z++)
      ;
    // Draw the eight octants
    plotPixel(x + r, y + s, color); // Octant 1
    plotPixel(x + s, y + r, color); // Octant 2
    plotPixel(x - s, y + r, color); // Octant 3
    plotPixel(x - r, y + s, color); // Octant 4
    plotPixel(x - r, y - s, color); // Octant 5
    plotPixel(x - s, y - r, color); // Octant 6
    plotPixel(x + s, y - r, color); // Octant 7
    plotPixel(x + r, y - s, color); // Octant 8
#define COLOR(r, g, b) (((r) & 0x1F) << 10 | ((g) & 0x1F) << 5 | ((b) & 0x1F))

#define SW 240
#define SH 160

//---Display control---//
#define DSPC *(u32 *)0x4000000
//---Video buffer---
#define VRAM (u16 *)0x6000000

//---Modes---//
#define MODE3 (u16)0x400
#define BG2 (u16)0x003

    s++;
    if (decisionOver2 <= 0) {
      decisionOver2 += 2 * s + 1; // Move down in y-direction
    } else {
      r--;
      decisionOver2 += 2 * (s - r) + 1; // Move down in both x and y
    }
  }
}
int main() {
  DSPC = (MODE3 | BG2);
  drawCircle(120, 80, 40, COLOR(23, 2, 14), VRAM);
  return 0;

  // approximate curve x^2+y^2=r^2
  // 2r = radius, or r << 2
  // start with center coords
  // lets go 120, 80
  //
}
