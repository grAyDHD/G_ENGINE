#include "ecs/components.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"
#include "ecs/entities.h"
#include "ecs/systems.h"
#include "math/math.h"

void playerInputHandler(ECS *ecs, int entityId) {
  AnimationComponent *animation = &ecs->components->animation[entityId];
  VelocityComponent *velocity = &ecs->components->velocity[entityId];
  AccelerationComponent *acceleration =
      &ecs->components->acceleration[entityId];

  if (keyDown(U)) {
    //if NOT a platformer
    if (!(ecs->entity[entityId].flag & ENABLE_GRAVITY)) {

      acceleration->ay -= INT_TO_FIXED(2048);
      animation->direction = UP;
      animation->state = WALK;
    }
  } else if (keyDown(D)) {
    acceleration->ay += INT_TO_FIXED(2048);
    animation->direction = DOWN;
    animation->state = WALK;
  } else if (keyDown(L)) {
    acceleration->ax -= INT_TO_FIXED(2048);
    animation->direction = LEFT;
    animation->state = WALK;
  } else if (keyDown(R)) {
    acceleration->ax += INT_TO_FIXED(2048);
    animation->direction = RIGHT;
    animation->state = WALK;
  }

  //if platformer
  if (keyTapped(A) && (ecs->entity[entityId].flag & ENABLE_GRAVITY & ON_GROUND)) {
    velocity->dy = -INT_TO_FIXED(64);
  }

  if (keyReleased(U | D | L | R)) {
    animation->frameNumber = 0;
    animation->state = IDLE;
    animation->keyframe = 0;

    acceleration->ax = 0;
    acceleration->ay = 0;
  }
}

// if (!keyDown(U | D | L | R)) {
//}
//

void patrolBehavior(ECS *ecs, int entityId) {
  // this should be a walking back and forth animagion.
  AIComponent *ai = &ecs->components->ai[entityId];
  // VelocityComponent *velocity = &ecs->components->velocity[entityId];
  AccelerationComponent *acceleration =
      &ecs->components->acceleration[entityId];
  AnimationComponent *animation = &ecs->components->animation[entityId];

  // p2 = distance
  ai->param2 = 120;

  if (animation->direction == LEFT) {
    acceleration->ax -= INT_TO_FIXED(128);

    ai->param1++;
    if (ai->param1 >= ai->param2) {
      ai->param1 = 0;
      animation->direction = RIGHT;
    }
  } else { // RIGHT
    acceleration->ax += INT_TO_FIXED(128);
    ai->param1++;
    if (ai->param1 >= ai->param2) {
      ai->param1 = 0;
      animation->direction = LEFT;
    }
  }
};
