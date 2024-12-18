#ifndef DRAW_H
#define DRAW_H

#include "typedefs.h"

#define RGB(r, g, b) (((r) & 0x1F) | ((g) & 0x1F) << 5 | ((b) & 0x1F) << 10)
#define dblClr(color) ((color) | (color << 16))

void plotPixel(int x, int y, unsigned short color);
void drawLine(Coordinate start, Coordinate end, unsigned short color);
void drawCircle(int x, int y, int radius, unsigned short color);
void fillCircle(int x, int y, int radius, unsigned short color);
void drawRect(Coordinate start, int width, int height, unsigned short color);

#endif
