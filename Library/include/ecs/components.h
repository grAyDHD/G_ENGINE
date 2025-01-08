#ifndef COMPONENTS_H
#define COMPONENTS_H

#define MAX_ENTITIES 10
#define MAX_INPUT_ENTITIES 4
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
