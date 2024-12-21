#ifndef PAINT_H
#define PAINT_H

#include "types.h"

#define GET_RED(color) (color & 0x1F)
#define GET_GREEN(color) ((color >> 5) & 0x1F)
#define GET_BLUE(color) ((color >> 10) & 0x1F)

void clearScreen(Brush brush);
void drawColorSliders(enum COLOR colorSelect, int red, int green, int blue);
void drawColorPreview(Brush brush, int color);

void drawGradientsGUI(enum COLOR colorSelect, Brush brush);

void fillCircle(int x, int y, int radius, unsigned short color);
void drawHorizontalLine(int x_start, int x_end, int y, unsigned short color);
void drawVerticalLine(int y_start, int y_end, int x, unsigned short color);

#endif // !PAINT
