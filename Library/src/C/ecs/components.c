#include "ecs/components.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"

// These are included as potential component behaviors,
// Likely to further categorize as library expands

void playerInputHandler(ECS *ecs, int entityId) {
  AnimationComponent *animation = &ecs->components->animation[entityId];
  VelocityComponent *velocity = &ecs->components->velocity[entityId];

  velocity->dx = 0;
  velocity->dy = 0;

  if (keyDown(U)) {
    velocity->dy = -1;
    animation->direction = UP;
    animation->state = WALK;
  } else if (keyDown(D)) {
    velocity->dy = 1;
    animation->direction = DOWN;
    animation->state = WALK;
  } else if (keyDown(L)) {
    velocity->dx = -1;
    animation->direction = LEFT;
    animation->state = WALK;
  } else if (keyDown(R)) {
    velocity->dx = 1;
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
