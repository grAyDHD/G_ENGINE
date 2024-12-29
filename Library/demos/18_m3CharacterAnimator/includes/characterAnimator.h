#ifndef CHARACTERANIMATOR_H
#define CHARACTERANIMATOR_H

#include "../build/Bedroom.h"
#include "../build/Robo.h"
#include "../build/RoboSmall.h"
#include "core/typedefs.h"

#define NUM_STATES 3
#define NUM_DIRECTIONS 4
#define MAX_ENTITIES 10

#define COMPONENT_PLAYER (1 << 0)
#define COMPONENT_POSITION (1 << 1)
#define COMPONENT_VELOCITY (1 << 2)
#define COMPONENT_ANIMATION (1 << 3)
#define COMPONENT_SPRITE (1 << 4)

// define PLAYER_ENTITY ENEMY_ENTITY INTERACTABLE_ENTITY
#define PLAYER_ENTITY                                                          \
  (COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_ANIMATION |             \
   COMPONENT_SPRITE)

typedef enum : int { DOWN = 0, UP, LEFT, RIGHT } DIRECTION;

typedef enum : int { IDLE = 0, WALK, TALK, RUN } STATE;

typedef struct {
  int entityID;
  int componentMask;
} Entity;

typedef struct {
  const void *spriteSheet; // pointer to active sprite 4 bytes
} SpriteComponent;

typedef struct {
  int frameNumber;
  DIRECTION direction;
  STATE state;
} AnimationComponent;

typedef struct {
  int x;
  int y;
} PositionComponent;

typedef struct {
  int dx;
  int dy;
} VelocityComponent;

typedef struct {
  PositionComponent position[MAX_ENTITIES];
  VelocityComponent velocity[MAX_ENTITIES];
  AnimationComponent animation[MAX_ENTITIES];
  SpriteComponent sprite[MAX_ENTITIES];
} ComponentManager;

extern void m3_Background(const void *src);

// extern void SpriteFrame32Bit(PositionComponent positionA,
//                             PositionComponent positionB, int frame,
//                             const void *image, int frameCount);
extern void SpriteFrame32Bit(PositionComponent *positionData,
                             AnimationComponent *animationData,
                             const void *image);

extern void clearSpriteFrame(int x, int y, int size, const void *image);

// to create player character:
#endif // CHARACTERANIMATOR_H
