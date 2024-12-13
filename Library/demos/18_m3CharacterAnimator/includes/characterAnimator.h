#ifndef CHARACTERANIMATOR_H
#define CHARACTERANIMATOR_H

#include "../build/Bedroom.h"

#include "../build/ChocoboJoyful.h"
#include "../build/ChocoboWalkingLeft.h"
#include "../build/ChocoboWalkingRight.h"

#include "../build/PkmnSlvrGirlWalkingDown.h"
#include "../build/PkmnSlvrGirlWalkingLeft.h"
#include "../build/PkmnSlvrGirlWalkingRight.h"
#include "../build/PkmnSlvrGirlWalkingUp.h"

#include "gfx.h"
#include "timer.h"
#include "typedefs.h"

#define NUM_STATES 3
#define NUM_DIRECTIONS 4

typedef struct {
  const void *spriteSheets[NUM_STATES][NUM_DIRECTIONS];
} CharacterSprites;

typedef enum { UP, DOWN, LEFT, RIGHT } DIRECTION;
typedef enum { IDLE, WALK, TALKING } STATE;

typedef struct {
  DIRECTION direction;
  STATE state;
  Coordinate coordinate;
  CharacterSprites sprites;       // struct array of pointers
  const void *currentSpriteSheet; // pointer to active sprite
} Character;

void initializeCharacterSprites(
    Character *character,
    const void *spriteSheets[NUM_STATES][NUM_DIRECTIONS]) {
  for (int state = 0; state < NUM_STATES; state++) {
    for (int direction = 0; direction < NUM_DIRECTIONS; direction++) {
      character->sprites.spriteSheets[state][direction] =
          spriteSheets[state][direction];
    }
  }

  character->currentSpriteSheet =
      character->sprites.spriteSheets[character->state][character->direction];
};

const void *chocoboSprites[NUM_STATES][NUM_DIRECTIONS] = {
    [IDLE] = {0, 0, 0, 0},
    [WALK] =
        {
            0, // up
            ChocoboWalkingLeftBitmap,
            0, // down
            ChocoboWalkingRightBitmap,
        },
    [TALKING] = {ChocoboJoyfulBitmap, ChocoboJoyfulBitmap, ChocoboJoyfulBitmap,
                 ChocoboJoyfulBitmap}};

extern void m3_Background(const void *src);
extern void SpriteFrame32Bit(int x, int y, int frame, const void *image,
                             int frameCount);
extern void fillFrameBackground(int x, int y, int size, const void *image);
extern void clearSpriteFrame(int x, int y, int size, const void *image);

#endif // CHARACTERANIMATOR_H
