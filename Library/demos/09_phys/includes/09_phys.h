#ifndef PHYS_H
#define PHYS_H

#include "core/typedefs.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "ecs/entities.h"
#include "graphics/draw.h"
#include "graphics/video.h"
#include "input/in.h"
#include "math/math.h"

// #define GRAVITY 1    // Gravity constant
#define MOVE_SPEED INT_TO_FIXED(90)
#define SHOOT_VEL INT_TO_FIXED(120)
#define JUMP_VEL INT_TO_FIXED(120)

extern int gravityDirection;
int gravityDirection = 1;
int BALL_SIZE = 4;

void playerInput(ECS *ecs, int entityId) {
  AnimationComponent *animation = &ecs->components->animation[entityId];
  VelocityComponent *velocity = &ecs->components->velocity[entityId];
  AccelerationComponent *acceleration =
      &ecs->components->acceleration[entityId];

  if (keyTapped(A) && (ecs->entity[0].flag & ON_GROUND)) {
    //    animation[0].state = JUMP;

    // will apply velocity later, checking for JUMP state and specific keyframe
    //    velocity[0].dy = -JUMP_VEL;

    // clear ON_GROUND
    ecs->entity[0].flag &= ~ON_GROUND;
  }

  /*
  if (keyDown(B)) {
    gravityDirection = -1;
  } else {
    gravityDirection = 1;
  }
  */

  if (keyDown(SL)) {
    velocity[0].dx = 0;
    velocity[0].dy = 0;
    acceleration[0].ax = 0;
    acceleration[0].ay = 0;

    // disable gravity/physics flag
  }
  // if (keyReleased) {
  // set gravity/physics flag
  // }

  if (keyDown(L)) {
    acceleration[0].ax = -MOVE_SPEED;
  } else if (keyDown(R)) {
    acceleration[0].ax = MOVE_SPEED;
  } else {
    acceleration[0].ax = 0;
  }

  if (keyTapped(LT)) {
    velocity[0].dx = -SHOOT_VEL;
    velocity[0].dy = -SHOOT_VEL;
  } else if (keyTapped(RT)) {
    velocity[0].dx = SHOOT_VEL;
    velocity[0].dy = -SHOOT_VEL;
  }
}

void drawBall(ECS *ecs, int entityId) {
  AnimationComponent *animation = &ecs->components->animation[entityId];
  PositionComponent *position = &ecs->components->position[entityId];
  PreviousPositionComponent *prevPos =
      &ecs->components->previousPosition[entityId];

  Coordinate corner = {.x = FIXED_TO_INT(position->x),
                       .y = FIXED_TO_INT(position->y)};

  // Previous position
  Coordinate prevCorner = {.x = FIXED_TO_INT(prevPos->x),
                           .y = FIXED_TO_INT(prevPos->y)};

  drawRect(prevCorner, BALL_SIZE, BALL_SIZE, 0x0000);

  switch (animation->state) {
  case JUMP:
    break;
  case LAND:
    break;
  case STATIC:
    drawRect(corner, BALL_SIZE, BALL_SIZE, 0x8f3d);
    break;
  default:
    drawRect(corner, BALL_SIZE, BALL_SIZE, 0x8f3d);
    break;
  }

  prevPos->x = position->x;
  prevPos->y = position->y;
}

int initBall(ECS *ecs, ComponentStorage *components) {
  int ball = createEntity(
      ecs, POSITION_COMPONENT | VELOCITY_COMPONENT | ACCELERATION_COMPONENT |
               INPUT_COMPONENT | HITBOX_COMPONENT | ANIMATION_COMPONENT |
               DRAWING_COMPONENT | PREVIOUS_POSITION_COMPONENT | ENABLE_INPUT |
               ENABLE_PHYSICS | PHYSICS_FLAG | DETECTS_COLLISIONS |
               TRIGGERS_COLLISIONS);

  components->position[ball] =
      (PositionComponent){.x = INT_TO_FIXED(120), .y = INT_TO_FIXED(80)};

  components->previousPosition[ball] = (PreviousPositionComponent){
      .x = INT_TO_FIXED(120), .y = INT_TO_FIXED(80)};

  components->velocity[ball] = (VelocityComponent){.dx = 0, .dy = 0};
  components->acceleration[ball] = (AccelerationComponent){.ax = 0, .ay = 0};
  components->input[ball].handleInput = playerInput;
  components->hitbox[ball] =
      (HitboxComponent){.width = BALL_SIZE, .height = BALL_SIZE};
  components->animation[ball] = (AnimationComponent){.state = STATIC,
                                                     .keyframe = 0,
                                                     .direction = 0,
                                                     .frameNumber = 0,
                                                     .keyframeInterval = 13};
  components->draw[ball].drawingRoutine = drawBall;

  return ball;
}

#endif
