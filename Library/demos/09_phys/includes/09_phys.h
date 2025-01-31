#include "ecs/components.h"
#include "math/math.h"
#ifndef PHYS_H
#include "ecs/ecs.h"
#include "graphics/video.h"
#include "input/in.h"

#define PHYS_H

// #define GRAVITY 1    // Gravity constant
#define MOVE_SPEED INT_TO_FIXED(20)
#define SHOOT_VEL 10 // Initial velocity when shooting with bumpers
#define JUMP_VELOCITY -10

int BALL_SIZE = 4;

extern int gravityDirection;

int gravityDirection = 1;
void applyGravity(ECS *obj) {
  obj->components->acceleration[0].ay = GRAVITY * gravityDirection;
}

int initBall(ECS *ecs, ComponentStorage *components) {
  int ball = createEntity(
      ecs, POSITION_COMPONENT | VELOCITY_COMPONENT | ACCELERATION_COMPONENT |
               INPUT_COMPONENT | HITBOX_COMPONENT | DRAWING_COMPONENT |
               ENABLE_INPUT | ENABLE_PHYSICS | PHYSICS_FLAG |
               DETECTS_COLLISIONS | TRIGGERS_COLLISIONS);

  components->position[ball] =
      (PositionComponent){.x = INT_TO_FIXED(120), .y = INT_TO_FIXED(80)};
  components->velocity[ball] = (VelocityComponent){.dx = 0, .dy = 0};
  components->acceleration[ball] = (AccelerationComponent){.ax = 0, .ay = 0};
  components->input[ball].handleInput = playerInputHandler;
  components->hitbox[ball] =
      (HitboxComponent){.width = BALL_SIZE, .height = BALL_SIZE};

  //  ecs->components->draw[ball].drawingRoutine = drawBall;

  return ball;
}

// void (*drawingRoutine)(ECS *ecs, int entityId);

void drawBall(ECS *ecs, int entityId) {
  // migrate drawing of ball into this function
}

void playerInput(ECS *ecs, int entityId) {

  if (keyTapped(A)) {
    ecs->components->velocity[0].dy = -SHOOT_VEL * gravityDirection;
  }

  if (keyDown(B)) {
    gravityDirection = -1;
  } else {
    gravityDirection = 1;
  }

  if (keyDown(SL)) {
    ecs->components->velocity[0].dx = 0;
    ecs->components->velocity[0].dy = 0;
    ecs->components->acceleration[0].ax = 0;
    ecs->components->acceleration[0].ay = 0;
  }

  if (keyDown(L)) {
    ecs->components->acceleration[0].ax = -MOVE_SPEED;
  } else if (keyDown(R)) {
    ecs->components->acceleration[0].ax = MOVE_SPEED;
  } else {
    ecs->components->acceleration[0].ax = 0;
  }

  if (keyDown(LT)) {
    ecs->components->velocity[0].dx = -SHOOT_VEL;
  } else if (keyDown(RT)) {
    ecs->components->velocity[0].dx = SHOOT_VEL;
  }
  if (keyTapped(LT)) {
    ecs->components->velocity[0].dy = -SHOOT_VEL;
  } else if (keyTapped(RT)) {
    ecs->components->velocity[0].dy = -SHOOT_VEL;
  }
}

#endif
