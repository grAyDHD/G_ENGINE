#ifndef GFX_H
#define GFX_H

#include "typedefs.h"

#define SW 240
#define SH 160

//---Display control---//
#define DSPC *(u32 *)0x4000000
//---Video buffer---
#define VRAM (u16 *)0x6000000

#define GREEN_SWAP *(u16 *)0x4000002
//---Modes---//
#define MODE0 0x0000
#define MODE1 0x0001
#define MODE2 0x0001
#define MODE3 0x0003
#define MODE4 0x0004
#define MODE5 0x0005

#define BG0 0x0100
#define BG1 0x0200
#define BG2 0x0400
#define BG3 0x0800
#define OBJ 0x1000

#define VCOUNT (*(volatile unsigned short *)0x04000006)

#define dblClr(color) ((color) | (color << 16))

extern void fillScreen(u32 clr);
void VBLANK();

#endif
