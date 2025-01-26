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

void handleCollisions(ECS *obj, int BALL_SIZE) {
  if (gravityDirection == 1) {
    if (obj->components->position[0].y >= SH - BALL_SIZE) {
      obj->components->position[0].y = SH - BALL_SIZE;
      obj->components->velocity[0].dy = -(obj->components->velocity[0].dy / 2);
      obj->entity[0].flag |= ON_GROUND;
    } else if (obj->components->position[0].y <= 0) {
      obj->components->velocity[0].dy = 0;
      obj->entity[0].flag &= ~ON_GROUND; // Clear ON_GROUND flag
    } else {
      obj->entity[0].flag &= ~ON_GROUND; // Clear ON_GROUND flag
    }
  } else if (gravityDirection == -1) {
    if (obj->components->position[0].y <= 0) {
      obj->components->position[0].y = 0;
      obj->components->velocity[0].dy = -(obj->components->velocity[0].dy / 2);
      obj->entity[0].flag |= ON_GROUND;
    } else if (obj->components->position[0].y >= SH - BALL_SIZE) {
      obj->components->position[0].y = SH - BALL_SIZE;
      obj->components->velocity[0].dy = 0;

      obj->entity[0].flag &= ~ON_GROUND; // Clear ON_GROUND flag
    } else {
      obj->entity[0].flag &= ~ON_GROUND; // Clear ON_GROUND flag
    }
  }

  if (obj->components->position[0].x < 0) {
    obj->components->position[0].x = 0;
    obj->components->velocity[0].dx = 0;
  }
  if (obj->components->position[0].x >= SW - BALL_SIZE) {
    obj->components->position[0].x = SW - BALL_SIZE;
    obj->components->velocity[0].dx = 0;
  }
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
