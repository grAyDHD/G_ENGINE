#ifndef M3TEXT_H
#define M3TEXT_H

#include "../build/Peaberry.h"
#include "typedefs.h"

// edit order to be width, height, yOffset
// make width into xOffset
// xOffset can be an int
// this ensures alignment of 2 byte struct
// final order of struct:
// xOffset, yOffset, height
// add all prior xOffset to current manually as lookup table
//
typedef struct {
  u16 xOffset;
  u16 yOffset;
  u16 width;
  u16 height;
} GlyphInfo;

GlyphInfo fontData[94] = {};

extern void copyGlyphToVRAM(int x, int y, GlyphInfo glyph,
                            const void *fontBitmap);
#endif // M3TEXT_H
