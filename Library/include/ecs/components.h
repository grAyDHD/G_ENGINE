#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "systems.h"

#define VELOCITY_SCALE 256 // For fixed-point math
#define MOVEMENT_SPEED 256 // 1 pixel per frame in fixed-point

void playerInputHandler(ECS *ecs, int entityId);
void patrolBehavior(ECS *ecs, int entityId);

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
  //  int offset; // y * image pixel width * 2 + x
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

#endif // !COMPONENTS_H
