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

#include "core/typedefs.h"

#define NUM_STATES 3
#define NUM_DIRECTIONS 4

typedef struct {
  const void *spriteSheets[NUM_STATES][NUM_DIRECTIONS];
} CharacterSprites;

typedef enum { UP, DOWN, LEFT, RIGHT } DIRECTION;
typedef enum { IDLE, WALK, TALK } STATE;

typedef struct {
  Coordinate coordinate; // two ints, 8 bytes?
  DIRECTION direction;   // 4 bytes if enum is int
  STATE state;           // 4 bytes, another enum
  CharacterSprites
      sprites; // struct array of pointers. each pointer is 4 bytes.
  const void *currentSpriteSheet; // pointer to active sprite 4 bytes

  int height;
  int width;
} Character;

//

const void *chocoboSprites[NUM_STATES][NUM_DIRECTIONS] = {
    [IDLE] = {0, 0, 0, 0},
    [WALK] =
        {
            0, // ChocoboWalkingUpBitmap
            0, // ChocoboWalkingDownBitmap
            ChocoboWalkingLeftBitmap,
            ChocoboWalkingRightBitmap,
        },
    [TALK] = {ChocoboJoyfulBitmap, ChocoboJoyfulBitmap, ChocoboJoyfulBitmap,
              ChocoboJoyfulBitmap}};

const void *silverGirlSprites[NUM_STATES][NUM_DIRECTIONS] = {
    [IDLE] = {PkmnSlvrGirlWalkingUpBitmap, PkmnSlvrGirlWalkingDownBitmap,
              PkmnSlvrGirlWalkingLeftBitmap, PkmnSlvrGirlWalkingRightBitmap},
    [WALK] = {PkmnSlvrGirlWalkingUpBitmap, PkmnSlvrGirlWalkingDownBitmap,
              PkmnSlvrGirlWalkingLeftBitmap, PkmnSlvrGirlWalkingRightBitmap},
    [TALK] = {PkmnSlvrGirlWalkingUpBitmap, PkmnSlvrGirlWalkingDownBitmap,
              PkmnSlvrGirlWalkingLeftBitmap, PkmnSlvrGirlWalkingRightBitmap}};

extern void m3_Background(const void *src);
extern void SpriteFrame32Bit(int x, int y, int frame, const void *image,
                             int frameCount);
extern void restoreFrameBackground(int x, int y, int size, const void *image);
extern void clearSpriteFrame(int x, int y, int size, const void *image);

#endif // CHARACTERANIMATOR_H
