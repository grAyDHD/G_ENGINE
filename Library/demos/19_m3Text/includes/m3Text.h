#ifndef M3TEXT_H
#define M3TEXT_H

#include "../build/Peaberry.h"

extern void copyGlyphToVRAM(int x, int y, GlyphInfo glyph,
                            const void *fontBitmap);
#endif // M3TEXT_H
