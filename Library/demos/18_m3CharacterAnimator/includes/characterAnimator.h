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

typedef enum { UP, DOWN, LEFT, RIGHT } DIRECTION;
typedef enum { IDLE, WALK, TALKING } STATE;

typedef struct {
  DIRECTION direction;
  STATE state;
  Coordinate coordinate;
} Character;

extern void SpriteFrame32Bit(int x, int y, int frame, const void *image,
                             int frameCount);
extern void fillFrameBackground(int x, int y, int size, const void *image);
extern void m3_Background(const void *src);
#endif // CHARACTERANIMATOR_H
