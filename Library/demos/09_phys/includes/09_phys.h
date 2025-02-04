#include "ecs/components.h"
#include "ecs/entities.h"
#include "math/math.h"
#ifndef PHYS_H
#include "ecs/ecs.h"
#include "graphics/video.h"
#include "input/in.h"

#define PHYS_H

// #define GRAVITY 1    // Gravity constant
#define MOVE_SPEED INT_TO_FIXED(90)
#define SHOOT_VEL INT_TO_FIXED(120)
#define JUMP_VEL INT_TO_FIXED(120)

extern int gravityDirection;
int gravityDirection = 1;
int BALL_SIZE = 4;

void playerInput(ECS *ecs, int entityId) {
  if (keyTapped(A) && (ecs->entity[0].flag & ON_GROUND)) {
    ecs->components->velocity[0].dy = -JUMP_VEL;
    // clear ON_GROUND
    ecs->entity[0].flag &= ~ON_GROUND;
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
    // disable gravity/physics flag
  }
  // if (keyReleased) {
  // set gravity/physics flag
  // }

  if (keyDown(L)) {
    ecs->components->acceleration[0].ax = -MOVE_SPEED;
  } else if (keyDown(R)) {
    ecs->components->acceleration[0].ax = MOVE_SPEED;
  } else {
    ecs->components->acceleration[0].ax = 0;
  }

  if (keyTapped(LT)) {
    ecs->components->velocity[0].dx = -SHOOT_VEL;
    ecs->components->velocity[0].dy = -SHOOT_VEL;
  } else if (keyTapped(RT)) {
    ecs->components->velocity[0].dx = SHOOT_VEL;
    ecs->components->velocity[0].dy = -SHOOT_VEL;
  }
}

void drawBall(ECS *ecs, int entityId) {
  // migrate drawing of ball into this function
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
  components->input[ball].handleInput = playerInput;
  components->hitbox[ball] =
      (HitboxComponent){.width = BALL_SIZE, .height = BALL_SIZE};
  components->draw[ball].drawingRoutine = drawBall;

  return ball;
}

// void (*drawingRoutine)(ECS *ecs, int entityId);

#endif
