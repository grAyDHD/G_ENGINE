#include "ecs/systems.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"
#include "core/typedefs.h"
#include "ecs/components.h"
#include "ecs/entities.h"
#include "math/math.h"

static int gravityDirection = 1;

void updateInputSystem(ECS *ecs, Entity *entity, InputComponent *input,
                       fixed_s32 deltaTime) {
  updateKeys();
  input[0].handleInput(ecs, 0, deltaTime);
}

void updateBehaviorSystem(ECS *ecs, Entity *entity, AIComponent *ai) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (entity[i].flag & AI_COMPONENT) {
      ai[i].aiBehavior(ecs, i);
    }
  }
};

// 1092 0000010001000100 0x0444
void updatePhysicsSystem(Entity *entity, VelocityComponent *velocity,
                         AccelerationComponent *acceleration,
                         fixed_s32 deltaTime) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (entity[id].flag & PHYSICS_FLAG) {
      acceleration[id].ay += GRAVITY;
    }

    velocity[id].dx += MULT(acceleration[id].ax, deltaTime);
    velocity[id].dy += MULT(acceleration[id].ay, deltaTime);
    acceleration[id].ax = 0;
    acceleration[id].ay = 0;
  }
}

void updateMovementSystem(Entity *entity, PositionComponent *position,
                          VelocityComponent *velocity, fixed_s32 deltaTime) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (entity[id].flag & VELOCITY_COMPONENT) {
      position[id].x += (MULT(velocity[id].dx, deltaTime));
      position[id].y += (MULT(velocity[id].dy, deltaTime));
    }
  }
}

// change to overlap sturct? same params, width and height
static inline HitboxComponent
checkForOverlap(PositionComponent *posA,
                VelocityComponent *velA, // no longer need for function
                HitboxComponent *hitA, PositionComponent *posB,
                VelocityComponent *velB, // no longer need for function
                HitboxComponent *hitB, fixed_s32 deltaTime) {

  int Ax1 = FIXED_TO_INT(posA->x);
  int Ax2 = Ax1 + hitA->width;
  int Ay1 = FIXED_TO_INT(posA->y);
  int Ay2 = Ay1 + hitA->height;
  int Bx1 = FIXED_TO_INT(posB->x);
  int Bx2 = Bx1 + hitB->width;
  int By1 = FIXED_TO_INT(posB->y);
  int By2 = By1 + hitB->height;

  HitboxComponent overlap = {0, 0};

  // only need to find 1 single overlapping corner to determine x and y depth
  if (Ax1 > Bx1 && Ax1 < Bx2 && Ay1 > By1 &&
      Ay2 < By2) { // check if top left corner of a is inside b
    overlap.width = Bx2 - Ax1;
    overlap.height = By2 - By1;
    return overlap;
  } else if (Ax2 > Bx1 && Ax2 < Bx2 && Ay1 > By1 &&
             Ay1 < By2) { // check if top right corner overlap
    overlap.width = Ax2 - Bx1;
    overlap.height = By2 - Ay1;
    return overlap;
  } else if (Ax1 > Bx1 && Ax1 < Bx2 && Ay2 > By1 &&
             Ay2 < By2) { // check bottom left of a for overlap
    overlap.width = Bx2 - Ax1;
    overlap.height = Ay2 - By1;
    return overlap;
  } else if (Ax2 > Bx1 && Ax2 < Bx2 && Ay2 > By1 &&
             Ay2 < By2) { // check bottom right of A for overlap
    overlap.width = Ax2 - Bx1;
    overlap.height = Ay2 - By1;
    return overlap;
  } else {
    return overlap;
    // if this returns height and width are both 0 meaning no
    // collision happened
  }
}

void updateCollisionSystem(Entity *entity, PositionComponent *position,
                           VelocityComponent *velocity, HitboxComponent *hitbox,
                           fixed_s32 deltaTime) {
  for (int idA = 0; idA < MAX_ENTITIES; idA++) {
    if (!(entity[idA].flag & DETECTS_COLLISIONS))
      continue;

    for (int idB = idA + 1; idB < MAX_ENTITIES; idB++) {
      if (!(entity[idB].flag & TRIGGERS_COLLISIONS))
        continue;

      HitboxComponent overlap = checkForOverlap(
          &position[idA], &velocity[idA], &hitbox[idA], &position[idB],
          &velocity[idB], &hitbox[idB], deltaTime);
      if (overlap.width > 0 && overlap.height > 0) {
        // set collision flag if detects collisions
        //  if idB is static, (add static collider flag)
        //  then, idA should be only eintity with position corrected.
        //  future idea: platform pushes plyer up if approached from below
      }
    }
  }
}

// resets collision flags
// for (int id = 0; id < max_entities; id++) {
//  entity[id].flag &= ~collision_detected;
//}
//

void updateanimationsystem(Entity *entity, AnimationComponent *animation) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (entity[id].flag & ANIMATION_COMPONENT) {
      animation->keyframe++;
      if (animation->keyframe >= animation->keyframeInterval) {
        animation->keyframe = 0;
        animation->frameNumber++;
        if (animation->frameNumber == 4) {
          animation->frameNumber = 0;
        }
      }
    }
  }
}

void updaterendersystem(ECS *ecs, Entity *entity, AnimationComponent *animation,
                        DrawingComponent *draw) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (entity[id].flag & ANIMATION_COMPONENT) {
      renderEntity(ecs, id);
    }

    if (entity[id].flag & DRAWING_COMPONENT) {
      draw[id].drawingRoutine(ecs, id);
    }
  }
};

#define horizontal_collision (1 << 30)
#define vertical_collision (1 << 31)
// entity[id].flag &= ~(horizontal_collision | vertical_collision);
