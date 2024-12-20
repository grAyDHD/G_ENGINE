#include "gfx/draw.h"
#include "core/typedefs.h"
#include "gfx/gfx.h"

void plotPixel(int x, int y, u16 clr) { ((u16 *)VRAM)[y * SW + x] = clr; }

void drawCircle(int x, int y, int radius, unsigned short color) {
  int r = radius;
  int s = 0;
  int decisionOver2 =
      1 - r; // Decision criterion divided by 2 evaluated at r=r, s=0

  while (r >= s) {

    // Draw the eight octants
    plotPixel(x + r, y + s, color); // Octant 1
    plotPixel(x + s, y + r, color); // Octant 2
    plotPixel(x - s, y + r, color); // Octant 3
    plotPixel(x - r, y + s, color); // Octant 4
    plotPixel(x - r, y - s, color); // Octant 5
    plotPixel(x - s, y - r, color); // Octant 6
    plotPixel(x + s, y - r, color); // Octant 7
    plotPixel(x + r, y - s, color); // Octant 8

    s++;
    if (decisionOver2 <= 0) {
      decisionOver2 += 2 * s + 1; // Move down in y-direction
    } else {
      r--;
      decisionOver2 += 2 * (s - r) + 1; // Move down in both x and y
    }
  }
}

void drawLine(Coordinate start, Coordinate end, u16 color) {
  int dx = (end.x - start.x);
  int dy = (end.y - start.y);
  int sx = (start.x < end.x) ? 1 : -1;
  int sy = (start.y < end.y) ? 1 : -1;
  int err = dx - dy;

  while (start.x != end.x || start.y != end.y) {
    plotPixel(start.x, start.y, color);
    int e2 = err * 2;

    if (e2 > -dy) {
      err -= dy;
      start.x += sx;
    }

    if (e2 < dx) {
      err += dx;
      start.y += sy;
    }
  }
  plotPixel(end.x, end.y, color);
}

void drawRect(Coordinate start, int width, int height, u16 color) {
  for (int i = start.x; i < start.x + width; i++) {
    plotPixel(i, start.y, color);
    plotPixel(i, start.y + height - 1, color);
  }
  for (int j = start.y; j < start.y + height; j++) {
    plotPixel(start.x, j, color);
    plotPixel(start.x + width - 1, j, color);
  }
}
