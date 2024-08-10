#include "draw.h"
#include "typedefs.h"

void drawLine(Coordinate start, Coordinate end, unsigned short color, unsigned short *vmem) {
    int dx = (end.x - start.x);
    int dy = (end.y - start.y);
    int sx = (start.x < end.x) ? 1 : -1;
    int sy = (start.y < end.y) ? 1 : -1;
    int err = dx - dy;

    while (start.x != end.x || start.y != end.y) {
        plotPixel(start.x, start.y, color, vmem);
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
    plotPixel(end.x, end.y, color, vmem);
}

void drawRect(Coordinate start, int width, int height, unsigned short color, unsigned short *vmem) {
    for (int i = start.x; i < start.x + width; i++) {
        plotPixel(i, start.y, color, vmem);
        plotPixel(i, start.y + height - 1, color, vmem);
    }
    for (int j = start.y; j < start.y + height; j++) {
        plotPixel(start.x, j, color, vmem);
        plotPixel(start.x + width - 1, j, color, vmem);
    }
}

/*
void drawLine(Coordinate start, Coordinate end, unsigned short color) {
    int x1 = start.x;
    int y1 = start.y;
    int x2 = end.x;
    int y2 = end.y;

    int dx = x2 - x1;
    int dy = y2 - y1;

    int sx = (dx >= 0) ? 1 : -1;
    int sy = (dy >= 0) ? 1 : -1;

    dx = (dx >= 0) ? dx : -dx;
    dy = (dy >= 0) ? dy : -dy;

    int err = dx - dy;
    int err2;

    while (1) {
        plotPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        err2 = 2 * err;
        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}
*/
