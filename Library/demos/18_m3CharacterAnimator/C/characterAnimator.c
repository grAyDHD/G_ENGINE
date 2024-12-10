#include "../build/Bedroom.h"

#include "../build/ChocoboJoyful.h"
#include "../build/ChocoboWalkingLeft.h"
#include "../build/ChocoboWalkingRight.h"

#include "../build/PkmnSlvrGirlWalkingDown.h"
#include "../build/PkmnSlvrGirlWalkingLeft.h"
#include "../build/PkmnSlvrGirlWalkingRight.h"
#include "../build/PkmnSlvrGirlWalkingUp.h"

#include "gfx.h"
#include "typedefs.h"

extern void m3_Background(const void *src);

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

typedef enum { UP, DOWN, LEFT, RIGHT } DIRECTION;
typedef enum { IDLE, WALK, TALKING } STATE;

typedef struct {
  DIRECTION direction;
  STATE state;
  Coordinate coordinate;
} Character;

extern void SpriteFrame32Bit(int x, int y, int frame, const void *image,
                             int frameCount);

int main() {
  DSPC = MODE3 | BG2;

  m3_Background(BedroomBitmap);
  SpriteFrame32Bit(64, 64, 3, ChocoboJoyfulBitmap, 4);

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
  while (1) {
    VBLANK();
  }
  return 0;
}
