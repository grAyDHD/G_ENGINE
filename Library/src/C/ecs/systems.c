#include "ecs/systems.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"
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

//   if (entity[id].flag & PHYSICS_FLAG) {
//   acceleration[id].ay += FIXED_MULTIPLY(GRAVITY, deltaTime);
//    }

// 1092 0000010001000100 0x0444
/*
#define MUL_FINTEGER(x, y)                                                     \
  (((x >> 16) * (y >> 16)) << 16) // clear lower bits to multiply integer
                                  // portion
#define MUL_FFRACTION(x, y) (((x & 0x0000FFFF) * (y & 0x0000FFFF)) >> 16)
#define MUL(x, y) (s32)(MUL_FINTEGER(x, y) | MUL_FFRACTION(x, y))
*/

// MUL(INT_TO_FIXED(256), 1092)

void updatePhysicsSystem(Entity *entity, VelocityComponent *velocity,
                         AccelerationComponent *acceleration,
                         fixed_s32 deltaTime) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
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
      if ((entity[id].flag & COLLISION_DETECTED)) {
        velocity[id].dx = 0;
        velocity[id].dy = 0;
      } else {
        position[id].x += (MULT(velocity[id].dx, deltaTime));
        position[id].y += (MULT(velocity[id].dy, deltaTime));
      }
    }
  }
}

static inline int
checkForCollision(PositionComponent *posA, VelocityComponent *velA,
                  HitboxComponent *hitA, PositionComponent *posB,
                  VelocityComponent *velB, HitboxComponent *hitB,
                  fixed_s32 deltaTime) {

  // Checking for what the NEXT position will be
  int Ax1 =
      posA->x + FIXED_TO_INT(velA->dx); //(FIXED_MULTIPLY(velA->dx, deltaTime));
  int Ax2 = Ax1 + hitA->width;
  int Ay1 =
      posA->y + FIXED_TO_INT(velB->dy); //(FIXED_MULTIPLY(velA->dy, deltaTime));
  int Ay2 = Ay1 + hitA->height;
  int Bx1 =
      posB->x + FIXED_TO_INT(velB->dx); //(FIXED_MULTIPLY(velB->dx, deltaTime));
  int Bx2 = Bx1 + hitB->width;
  int By1 =
      posB->y + FIXED_TO_INT(velB->dy); //(FIXED_MULTIPLY(velB->dy, deltaTime));
  int By2 = By1 + hitB->height;

  return (Bx2 > Ax1 && Bx1 < Ax2 && By2 > Ay1 && By1 < Ay2);
}

void updateCollisionSystem(Entity *entity, PositionComponent *position,
                           VelocityComponent *velocity, HitboxComponent *hitbox,
                           fixed_s32 deltaTime) {
  // Resets collision flags
  for (int id = 0; id < MAX_ENTITIES; id++) {
    entity[id].flag &= ~COLLISION_DETECTED;
  }

  for (int idA = 0; idA < MAX_ENTITIES; idA++) {
    if (entity[idA].flag & DETECTS_COLLISIONS) {
      // for every entity that can detect collisions,
      for (int idB = idA + 1; idB < MAX_ENTITIES; idB++) {
        if (entity[idB].flag & TRIGGERS_COLLISIONS) {
          //  check for collisions with any entity that triggers them
          if (checkForCollision(&position[idA], &velocity[idA], &hitbox[idA],
                                &position[idB], &velocity[idB], &hitbox[idB],
                                deltaTime)) {
            entity[idA].flag |= COLLISION_DETECTED;
            entity[idB].flag |= COLLISION_DETECTED;
          }
        }
      }
    }
  }
}

void updateAnimationSystem(Entity *entity, AnimationComponent *animation) {
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

void updateRenderSystem(ECS *ecs, Entity *entity, AnimationComponent *animation,
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

#define HORIZONTAL_COLLISION (1 << 30)
#define VERTICAL_COLLISION (1 << 31)
// entity[id].flag &= ~(HORIZONTAL_COLLISION | VERTICAL_COLLISION);
