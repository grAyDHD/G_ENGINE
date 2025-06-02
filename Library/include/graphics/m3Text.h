#ifndef TEXT_H
#define TEXT_H

#include "core/typedefs.h"

typedef struct {
  u16 xOffset;
  u16 width;
} GlyphInfo;

extern GlyphInfo fontData[94];
extern void printGlyph(int x, int y, const void *fontBitmap, GlyphInfo *fontDataIndex);

int getFontDataIndex(char c);
void renderChar(int *x, int *y, char c);
void printString(int x, int y, const char *text);
void gprintf(int x, int y, const char *format, u32 arg);

#endif

