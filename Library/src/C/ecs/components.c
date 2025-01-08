#include "ecs/components.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"

// These are included as potential component behaviors,
// Likely to further categorize as library expands

void playerInputHandler(ECS *ecs, int entityId) {
  AnimationComponent *animation = &ecs->components->animation[entityId];
  VelocityComponent *velocity = &ecs->components->velocity[entityId];
  AccelerationComponent *acceleration =
      &ecs->components->acceleration[entityId];

  if (keyDown(U)) {
    acceleration->ay -= 1;
    animation->direction = UP;
    animation->state = WALK;
  } else if (keyDown(D)) {
    acceleration->ay += 1;
    animation->direction = DOWN;
    animation->state = WALK;
  } else if (keyDown(L)) {
    acceleration->ax -= 1;
    animation->direction = LEFT;
    animation->state = WALK;
  } else if (keyDown(R)) {
    acceleration->ax += 1;
    animation->direction = RIGHT;
    animation->state = WALK;
  }

  if (keyReleased(U | D | L | R)) {
    if (!keyDown(U | D | L | R)) {
      animation->frameNumber = 0;
      animation->state = IDLE;
      animation->keyframe = 0;
    }
  }
}

void patrolBehavior(ECS *ecs, int entityId) {
  // this should be a walking back and forth animagion.
  AIComponent *ai = &ecs->components->ai[entityId];
  VelocityComponent *velocity = &ecs->components->velocity[entityId];
  AnimationComponent *animation = &ecs->components->animation[entityId];

  velocity->dx = 0;
  velocity->dy = 0;

  // p2 = distance
  ai->param2 = 40;

  if (animation->direction == LEFT) {
    velocity->dx = -1;
    ai->param1++;
    if (ai->param1 >= ai->param2) {
      ai->param1 = 0;
      animation->direction = RIGHT;
    }
  } else { // RIGHT
    velocity->dx = 1;
    ai->param1++;
    if (ai->param1 >= ai->param2) {
      ai->param1 = 0;
      animation->direction = LEFT;
    }
  }
};

/*

// components.h
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "entities.h"

// Forward declare ECS for function pointers
typedef struct ECS ECS;

typedef struct {
  int x;
  int y;
} PositionComponent;

typedef struct {
  int dx;
  int dy;
} VelocityComponent;

typedef struct {
  int ax;
  int ay;
} AccelerationComponent;

typedef struct {
  int width;
  int height;
} HitboxComponent;

typedef enum : int { DOWN = 0, UP, LEFT, RIGHT } DIRECTION;
typedef enum : int { IDLE = 0, WALK, TALK, RUN } STATE;

typedef struct {
  int frameNumber;
  DIRECTION direction;
  STATE state;
  int keyframe;
  int keyframeInterval;
} AnimationComponent;

typedef struct {
  const void *spriteSheet;
} SpriteComponent;

typedef struct {
  void (*handleInput)(Entity* entity, int entityId);
} InputComponent;

typedef struct {
  void (*aiBehavior)(Entity* entity, int entityId);
  int param1;
  int param2;
} AIComponent;

typedef struct {
  void (*drawingRoutine)(Entity* entity, int entityId);
} DrawingComponent;

// Function declarations
void playerInputHandler(Entity* entity, int entityId);
void patrolBehavior(Entity* entity, int entityId);

#endif // COMPONENTS_H

*/
