#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "core/typedefs.h"

#define MAX_ENTITIES 10
#define MAX_INPUT_ENTITIES 4

typedef s32 fixed_s32;
typedef struct ECS ECS;

typedef struct {
  fixed_s32 x;
  fixed_s32 y;
} PositionComponent;

typedef struct {
  fixed_s32 dx;
  fixed_s32 dy;
} VelocityComponent;

typedef struct {
  fixed_s32 ax;
  fixed_s32 ay;
} AccelerationComponent;

typedef struct {
  int width;
  int height;
  //  int offset; // y * image pixel width * 2 + x
} HitboxComponent;

typedef enum : int { DOWN = 0, UP, LEFT, RIGHT } DIRECTION;
typedef enum : int { IDLE = 0, WALK, TALK, RUN, STATIC = 10, JUMP, LAND } STATE;

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
  void (*handleInput)(ECS *ecs, int entityId);
} InputComponent;

typedef struct {
  void (*aiBehavior)(ECS *ecs, int entityId); // Custom behavior
  int param1;
  int param2;
} AIComponent;

typedef struct {
  void (*drawingRoutine)(ECS *ecs, int entityId);
} DrawingComponent;

typedef struct ComponentStorage {
  PositionComponent position[MAX_ENTITIES];
  VelocityComponent velocity[MAX_ENTITIES];
  AccelerationComponent acceleration[MAX_ENTITIES];
  HitboxComponent hitbox[MAX_ENTITIES];
  AnimationComponent animation[MAX_ENTITIES];
  SpriteComponent sprite[MAX_ENTITIES];
  InputComponent input[MAX_INPUT_ENTITIES];
  AIComponent ai[MAX_ENTITIES];
  DrawingComponent draw[MAX_ENTITIES];
} ComponentStorage;

#endif // !COMPONENTS_H
