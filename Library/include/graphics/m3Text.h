#ifndef TEXT_H
#define TEXT_H

#include "core/typedefs.h"

typedef struct {
  u16 xOffset;
  u16 width;
} GlyphInfo;

extern GlyphInfo fontData[94];
extern void printGlyphColored(int x, int y, const void *fontBitmap, GlyphInfo *fontDataIndex, u16 currentTextColor);

int getCurrentTextColor();

void setTextColorRGB(int r, int g, int b);
void setTextColor(u16 color);
void renderChar(int *x, int *y, char c);
void printString(int x, int y, const char *text);
extern void gprintf(int x, int y, const char *format, u32 arg);
int getFontDataIndex(char c);

#endif

