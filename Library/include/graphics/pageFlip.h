#ifndef PAGEFLIP_H
#define PAGEFLIP_H

#include "core/typedefs.h"

#define PALETTE ((volatile u16 *)0x5000000)
#define PAGE_OFFSET 0xA000
#define FLIP 0x0010

extern u16 *bufferPage;

void setPaletteColor(u8 index, u16 color);

u16 *pageFlip();

void m4_plotPixel(int x, int y, u8 clrid);

#endif
