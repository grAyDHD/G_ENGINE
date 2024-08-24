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

#define REG_BG2PA *(volatile u16 *)0x4000020 // Horizontal scaling factor
#define REG_BG2PB *(volatile u16 *)0x4000022 // Horizontal shearing factor
#define REG_BG2PC *(volatile u16 *)0x4000024 // Vertical shearing factor
#define REG_BG2PD *(volatile u16 *)0x4000026 // Vertical scaling factor

#define REG_BG2X *(volatile s32 *)0x4000028 // Horizontal offset (using s32)
#define REG_BG2Y *(volatile s32 *)0x400002C // Vertical offset (using s32)

#define REG_TM2D *(volatile u16 *)0x4000108

#define IN_IWRAM __attribute__((section(".iwram")))
#define IN_EWRAM __attribute__((section(".ewram")))

// bit 0, vblank status, 0 not in vblank, 1 = in VBlank
// bit 3, VBlank interrupt enable
// poll DSPSTAT once set
// loop until vblank status bit is set

#define DSPSTAT (*(volatile unsigned short *)0x04000004)
#define VCOUNT                                                                 \
  (*(volatile unsigned short                                                   \
         *)0x04000006) // current scanline number being drawn, 228
                       // scanlines/frame, 0indexed
//---rename unsigned short and long---

void waitVBLANK();

#endif
