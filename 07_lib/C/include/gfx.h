#ifndef GFX_H
#define GFX_H

#include "typedefs.h"

#define SW 240
#define SH 160

//---Display control---//
#define DSPC *(u32 *)0x4000000
//---Video buffer---
#define VRAM (u16 *)0x6000000

//---Modes---//
#define MODE3 (u16)0x400
#define BG2 (u16)0x003

#define VCOUNT (*(volatile unsigned short *)0x04000006)
void VBLANK();

#endif
