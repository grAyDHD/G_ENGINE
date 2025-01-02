#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "systems.h"

typedef struct {
  int x;
  int y;
} PositionComponent;

typedef struct {
  int dx;
  int dy;
} VelocityComponent;

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
  const void *spriteSheet; // pointer to active sprite 4 bytes
} SpriteComponent;

typedef struct {
  void (*handleInput)(ECS *ecs, int entityId); // Custom behavior
} InputComponent;

typedef struct {
  void (*aiBehavior)(ECS *ecs, int entityId); // Custom behavior
  int param1;
  int param2;
} AIComponent;

void playerInputHandler(ECS *ecs, int entityId);
void patrolBehavior(ECS *ecs, int entityId);
#endif // !COMPONENTS_H
