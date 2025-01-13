#include "ecs/components.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"
#include "ecs/systems.h"
#include "math/math.h"

// These are included as potential component behaviors,
// Likely to further categorize as library expands

void playerInputHandler(ECS *ecs, int entityId, fixed_s32 deltaTime) {
  AnimationComponent *animation = &ecs->components->animation[entityId];
  VelocityComponent *velocity = &ecs->components->velocity[entityId];
  AccelerationComponent *acceleration =
      &ecs->components->acceleration[entityId];
  // 1092 0000010001000100 0x0444

  if (keyDown(U)) {
    acceleration->ay -= INT_TO_FIXED(64);
    animation->direction = UP;
    animation->state = WALK;
  } else if (keyDown(D)) {
    acceleration->ay += INT_TO_FIXED(64);
    animation->direction = DOWN;
    animation->state = WALK;
  } else if (keyDown(L)) {
    acceleration->ax -= INT_TO_FIXED(64);
    animation->direction = LEFT;
    animation->state = WALK;
  } else if (keyDown(R)) {
    acceleration->ax += INT_TO_FIXED(64);
    animation->direction = RIGHT;
    animation->state = WALK;
  }

  if (keyReleased(U | D | L | R)) {
    animation->frameNumber = 0;
    animation->state = IDLE;
    animation->keyframe = 0;

    acceleration->ax = 0;
    acceleration->ay = 0;
    velocity->dx = 0;
    velocity->dy = 0;
  }
}

// if (!keyDown(U | D | L | R)) {
//}
//

// ecs->components->acceleration[entityId].ay -=
//   FIXED_MULTIPLY(INT_TO_FIXED(8), deltaTime);
//    ecs->components->acceleration[entityId].ay +=
//      FIXED_MULTIPLY(INT_TO_FIXED(8), deltaTime);
// ecs->components->acceleration[entityId].ax -=
//   FIXED_MULTIPLY(INT_TO_FIXED(8), deltaTime);
// ecs->components->acceleration[entityId].ax +=
//    FIXED_MULTIPLY(INT_TO_FIXED(8), deltaTime);

// ecs->components->acceleration[entityId].ay -= FIXED_QUARTER; //-= 1;
// ecs->components->acceleration[entityId].ay += FIXED_QUARTER; //+= 10;
// ecs->components->acceleration[entityId].ax -= FIXED_QUARTER; //-= 1;
// ecs->components->acceleration[entityId].ax += FIXED_QUARTER; //+= 10;
//
//    acceleration->ay -= 64; // FIXED_MULTIPLY(INT_TO_FIXED(3), deltaTime);
//    acceleration->ay += 256; // FIXED_MULTIPLY(INT_TO_FIXED(3),
//    deltaTime);
//    acceleration->ax -= 64; // FIXED_MULTIPLY(INT_TO_FIXED(3), deltaTime);
//    acceleration->ax += 256; // FIXED_MULTIPLY(INT_TO_FIXED(3),
//    deltaTime);

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
