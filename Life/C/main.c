//--------------------------By:3DSage-------------------------------------------
//               https://www.youtube.com/c/3DSage
//------------------------------------------------------------------------------

//---#defines---
#include "gfx.h"
//---Math functions---
#include <math.h>

//---Global variables---
#define GBA_SW 160                                        // actual GBA screen width
#define SW     160                                        // game screen width
#define SH     40                                        // game screen height
#define VIRTUAL_WIDTH 320                                 // virtual screen width (double the GBA width)
#define RGB(r,g,b) ((r)+((g)<<5)+((b)<<10))               // 15 bit, 0-31, 5bit=r, 5bit=g, 5bit=b 

int lastFr = 0, FPS = 0;                                  // for frames per second
int screenXOffset = 0;                                    // horizontal offset for screen shifting

typedef struct {                                          // player struct
    int x, y;                                             // position
} Player; Player P;

void clearBackground() {
    int x, y;
    for (x = 0; x < SW; x++) {
        for (y = 0; y < SH; y++) { 
            VRAM[y * GBA_SW + x] = RGB(8, 12, 16);        // clear all 120x80 pixels with blue color
        }
    }
}

void buttons() {
    if (KEY_R) { P.x += 3; if (P.x > SW - 1) { P.x = SW - 1; }}  // Move right
    if (KEY_L) { P.x -= 3; if (P.x < 0) { P.x = 0; }}            // Move left
    if (KEY_U) { P.y -= 3; if (P.y < 0) { P.y = 0; }}            // Move up
    if (KEY_D) { P.y += 3; if (P.y > SH - 1) { P.y = SH - 1; }}  // Move down

    // Shift the display left (LS) or right (RS)
    if (KEY_LS) {
        screenXOffset -= SCREEN_WIDTH;
        if (screenXOffset < 0) screenXOffset = 0;  // Prevent going past the left edge
    }
    if (KEY_RS) {
        screenXOffset += SCREEN_WIDTH;
        if (screenXOffset > VIRTUAL_WIDTH - SCREEN_WIDTH)
            screenXOffset = VIRTUAL_WIDTH - SCREEN_WIDTH;  // Prevent going past the right edge
    }

    // Update the affine matrix with the new horizontal offset
    REG_BG2X = screenXOffset << 8;
}

void init() {
    P.x = 70; P.y = 35;  // Initialize player position

    // Set up the affine transformation matrix
    REG_BG2PA = 256 / 4;  // Scale horizontally by 0.25x
    REG_BG2PD = 256 / 4;  // Scale vertically by 0.25x
    REG_BG2X = 0;         // Start at the top-left corner (x-offset)
    REG_BG2Y = 0;         // Start at the top-left corner (y-offset)
}

int main() {
    // Init Mode 5
 *(u16*)0x4000000 = 0x405;                                           //mode 5 background 2
 *(u16*)0x400010A = 0x82;                                            //enable timer for fps
 *(u16*)0x400010E = 0x84;                                            //cnt timer overflow

    init();  // Initialize game variables

    while (1) {
        if (REG_TM2D >> 12 != lastFr) {  // Draw 15 frames a second
            clearBackground();  // Clear background
            buttons();  // Handle button input

            VRAM[P.y * GBA_SW + P.x] = RGB(0, 31, 0);  // Draw player dot

            // FPS counter
            VRAM[15] = 0;
            VRAM[FPS] = RGB(31, 31, 0);  // Draw FPS
            FPS += 1;
            if (lastFr > REG_TM2D >> 12) { FPS = 0; }  // Reset counter
            lastFr = REG_TM2D >> 12;

            // Swap buffers
            while (*Scanline < 160) {}  // Wait for all scanlines
            if (DISPCNT & BACKB) {
                DISPCNT &= ~BACKB;
                VRAM = (u16*)VRAM_B;  // Back buffer
            } else {
                DISPCNT |= BACKB;
                VRAM = (u16*)VRAM_F;  // Front buffer
            }
        }
    }
}
