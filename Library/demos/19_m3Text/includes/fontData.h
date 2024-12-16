#ifndef FONT_DATA_H
#define FONT_DATA_H

#include "../build/Peaberry.h"
#include "typedefs.h"

typedef struct {
  u16 xOffset;
  u16 yOffset;
  u16 width;
  u16 height;
} GlyphInfo;

extern GlyphInfo fontData[94];

extern void copyGlyphToVRAM(int x, int y, const void *fontBitmap);

#endif
