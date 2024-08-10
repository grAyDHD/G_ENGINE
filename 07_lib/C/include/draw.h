#ifndef DRAW_H
#define DRAW_H

#include "gfx.h"
#include "typedefs.h"

void plotPixel(
        int x,
        int y,
        unsigned short color,
        unsigned short *vmem
        );
void drawLine(
        Coordinate start,
        Coordinate end,
        unsigned short color,
        unsigned short *vmem
        );
void drawRect(
        Coordinate start,
        int width,
        int height,
        unsigned short color,
        unsigned short *vmem
        );

#endif
