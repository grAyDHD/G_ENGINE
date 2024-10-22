#include "engine.h"
#include "gfx.h"
#include "in.h"

//---rename unsigned short and long---
// typedef unsigned long u32;
//---Video buffer---
// u16 *VRAM;
#define VRAM_F (u16 *)0x6000000
#define VRAM_B (u16 *)0x600A000
#define DISPCNT *(u32 *)0x4000000
#define BACKB 0x10
volatile u16 *Scanline = (volatile u16 *)0x4000006;
//---Scale mode 5 screen---
#define REG_BG2PA *(volatile unsigned short *)0x4000020
#define REG_BG2PD *(volatile unsigned short *)0x4000026
//---Timer for FPS---
#define REG_TM2D *(volatile u16 *)0x4000108
//--Fast Iwram and Ewram
#define IN_IWRAM __attribute__((section(".iwram")))
#define IN_EWRAM __attribute__((section(".ewram")))

#define GBA_SW 160       // actual gba screen width
#define SWR 120          // game screen width
#define SHR 80           // game screen height
int lastFr = 0, FPS = 0; // for frames per second

typedef struct // player
{
  int x, y; // position
} Player;
Player P;

void clearBackground() {
  int x, y;
  for (x = 0; x < SW; x++) { // rgb values 0-31
    for (y = 0; y < SH; y++) {
      (u16 *)VRAM[y * GBA_SW + x] = RGB(8, 12, 16);
      // void plotPixel(int x, int y, u16 clr) { ((u16 *)VRAM)[y * SW + x] =
      // clr; } plotPixel(x, y, RGB(11,0,1));
    } // clear all 120x80 pixels
  }
}

void buttons() // buttons to press
{
  if (key_is_down(L)) {
    P.x += 3;
    if (P.x > SW - 1) {
      P.x = SW - 1;
    }
  } // move right
  if (key_is_down(R)) {
    P.x -= 3;
    if (P.x < 0) {
      P.x = 0;
    }
  } // move left
  if (key_is_down(UP)) {
    P.y -= 3;
    if (P.y < 0) {
      P.y = 0;
    }
  } // move up
  if (key_is_down(DN)) {
    P.y += 3;
    if (P.y > SH - 1) {
      P.y = SH - 1;
    }
  } // move down
}

void init() {
  P.x = 70;
  P.y = 35; // init player
}

int main() {
  int x, y;
  // Init mode
  // 5------------------------------------------------------------------
  DSPC = MODE5 | BG2;

  *(u16 *)0x400010A = 0x82; // enable timer for fps
  *(u16 *)0x400010E = 0x84; // cnt timer overflow

  // scale small mode 5 screen to full
  // screen-------------------------------------
  REG_BG2PA = 256 / 2; // 256=normal 128=scale
  REG_BG2PD = 256 / 2; // 256=normal 128=scale

  init(); // init game variables

  while (1) {
    if (REG_TM2D >> 12 != lastFr) // draw 15 frames a second
    {
      clearBackground(); // clear
                         // background-----------------------------------------
      buttons();         // button input

      VRAM[P.y * GBA_SW + P.x] = RGB(0, 31, 0); // draw player dot

      // frames per
      // second----------------------------------------------------------
      VRAM[15] = 0;
      VRAM[FPS] = RGB(31, 31, 0); // draw fps
      FPS += 1;
      if (lastFr > REG_TM2D >> 12) {
        FPS = 0;
      } // increase frame
      lastFr = REG_TM2D >> 12; // reset counter

      // swap
      // buffers---------------------------------------------------------------
      while (*Scanline < 160) {
      } // wait all scanlines
      if (DISPCNT & BACKB) {
        DISPCNT &= ~BACKB;
        VRAM = (u16 *)VRAM_B;
      } // back  buffer
      else {
        DISPCNT |= BACKB;
        VRAM = (u16 *)VRAM_F;
      } // front buffer
    }
  }
}
