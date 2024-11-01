#include "draw.h"
#include "gfx.h"
#include "typedefs.h"

void plotPixel(int x, int y, u16 clr) { ((u16 *)VRAM)[y * SW + x] = clr; }

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
