#include "../includes/characterAnimator.h"

extern void m3_Background(const void *src);

extern void SpriteFrame32Bit(int x, int y, int frame, const void *image,
                             int frameCount);

extern void fillFrameBackground(int x, int y, int size, const void *image);

int main() {
  DSPC = MODE3 | BG2;

  m3_Background(BedroomBitmap);

  while (1) {
    VBLANK();
    SpriteFrame32Bit(64, 64, 0, ChocoboJoyfulBitmap, 4);
    fillFrameBackground(64, 64, 32, BedroomBitmap);
    simpleWait(100);

    VBLANK();
    SpriteFrame32Bit(64, 64, 1, ChocoboJoyfulBitmap, 4);
    //  fillFrameBackground(64, 64, 32, BedroomBitmap);
    simpleWait(100);

    VBLANK();
    SpriteFrame32Bit(64, 64, 2, ChocoboJoyfulBitmap, 4);
    //  fillFrameBackground(64, 64, 32, BedroomBitmap);
    simpleWait(100);

    VBLANK();
    SpriteFrame32Bit(64, 64, 3, ChocoboJoyfulBitmap, 4);
    // fillFrameBackground(64, 64, 32, BedroomBitmap);
    simpleWait(100);
  }

  while (1) {
    VBLANK();
  }
  return 0;
}

// NPC
//  DIRECTION, Up or down when walking, with a check for position when to turn
//  around
//*  NPC only moves up and down, but direction can become left or right if
// player interacts from that direction */

// PLAYER
//*  DIRECTION updates to last dpad input.
//
//

// create struct for character,to extend to player and npc
// Start by drawing a background, with cache for each player.
// decide upon sprites to use until making own
// cfind asm func to draw animation frame, adapt to meet needs of this program
// NPC BEHAVIOR:
// draw at npc.coordinates
// if direction + UP and sstate = up, y- 1
// check step counter, of = 2, state now is DOWN.  then y +1.
// reset counter to zero immediately on changing direction
// PLAYER BEHAVIORE:
// if dpad is down, add 8 directionmovement?
// if not, last dpad input  is DIRECTION
// if any dpad is down, STATE is walking
// if facing exact square of NPC and pressing A, TALKING
// when no DPAD is pressed STATE is idle.
//
// calculate grid based on character size?
// have asm function take grid into accountto take player x and yproperly, is
// this necessary?  might mike fefinements easier. find ggood but simple 3
// four frame walking animation
