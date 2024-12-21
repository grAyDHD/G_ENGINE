#ifndef DRAW_H
#define DRAW_H

#include "core/typedefs.h"

#define RGB(r, g, b) (((r) & 0x1F) | ((g) & 0x1F) << 5 | ((b) & 0x1F) << 10)
#define dblClr(color) ((color) | (color << 16))

void plotPixel(int x, int y, unsigned short color);
void drawLine(Coordinate start, Coordinate end, unsigned short color);
void drawCircle(int x, int y, int radius, unsigned short color);
void fillCircle(int x, int y, int radius, unsigned short color);
void drawRect(Coordinate start, int width, int height, unsigned short color);

extern void fillScreen(u32 clr);
extern void m3_Background(const void *src);
extern void SpriteFrame(int x, int y, int frame, const void *image,
                        int frameCount); // project 15 implementation

// #define BG2_X_ROTATION *(volatile u16 *)0x4000022 // Skewing/rotation in X
// #define BG2_Y_ROTATION *(volatile u16 *)0x4000024 // Skewing/rotation in Y

#define BG2_X_SCALING *(volatile u16 *)0x4000020 // Scaling for X axis
#define BG2_Y_SCALING *(volatile u16 *)0x4000026 // Scaling for Y axis

#define BG2_X_SCROLLING *(volatile u32 *)0x4000028
#define BG2_Y_SCROLLING *(volatile u32 *)0x400002C

void m5_plotPixel(int x, int y, u16 clr);

#endif
