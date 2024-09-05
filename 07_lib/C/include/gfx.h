#ifndef GFX_H
#define GFX_H

#include "typedefs.h"

#define COLOR(r, g, b) (((r) & 0x1F) << 10 | ((g) & 0x1F) << 5 | ((b) & 0x1F))

#define SW 240
#define SH 160

//---Display control---//
#define DSPC *(u32 *)0x4000000
//---Video buffer---
#define VRAM (u16 *)0x6000000

//---Modes---//
#define MODE3 (u16)0x400
#define BG2 (u16)0x003

//---Scale mode 5 screen---//

#define DSPSTAT (*(volatile unsigned short *)0x04000004)
#define VCOUNT                                                                 \
  (*(volatile unsigned short                                                   \
         *)0x04000006) // current scanline number being drawn, 228
                       // scanlines/frame, 0indexed
//---rename unsigned short and long---

void waitVBLANK();

#endif
