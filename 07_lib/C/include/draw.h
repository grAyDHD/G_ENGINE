#ifndef DRAW_H
#define DRAW_H

#include "typedefs.h"

void plotPixel(int x, int y, unsigned short color);
void drawLine(Coordinate start, Coordinate end, unsigned short color);
void drawRect(Coordinate start, int width, int height, unsigned short color);

#endif
