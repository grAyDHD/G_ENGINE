#include "ecs/systems.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"
#include "ecs/components.h"
#include "ecs/entities.h"

#define GRAVITY 1
//(1 << 8) // Gravity constant
// delta time = 4.  frame

static int gravityDirection = 1;

void updateInputSystem(ECS *ecs, Entity *entity, InputComponent *input) {
  updateKeys();
  input[0].handleInput(ecs, 0);
}

void updateBehaviorSystem(ECS *ecs, Entity *entity, AIComponent *ai) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (entity[i].flag & AI_COMPONENT) {
      ai[i].aiBehavior(ecs, i);
    }
  }
};

void updatePhysicsSystem(Entity *entity, VelocityComponent *velocity,
                         AccelerationComponent *acceleration) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (entity[id].flag & PHYSICS_FLAG) {
      acceleration[id].ay += GRAVITY;
    }

    velocity[id].dx += acceleration[id].ax;
    velocity[id].dy += acceleration[id].ay;

    acceleration[id].ax = 0;
    acceleration[id].ay = 0;
  }
}

static inline int
checkForCollision(PositionComponent *posA, VelocityComponent *velA,
                  HitboxComponent *hitA, PositionComponent *posB,
                  VelocityComponent *velB, HitboxComponent *hitB) {
  int Ax1 = posA->x + velA->dx;
  int Ax2 = Ax1 + hitA->width;
  int Ay1 = posA->y + velA->dy;
  int Ay2 = Ay1 + hitA->height;
  int Bx1 = posB->x;
  int Bx2 = Bx1 + hitB->width;
  int By1 = posB->y;
  int By2 = By1 + hitB->height;
  return (Bx2 > Ax1 && Bx1 < Ax2 && By2 > Ay1 && By1 < Ay2);
}

void updateCollisionSystem(Entity *entity, PositionComponent *position,
                           VelocityComponent *velocity,
                           HitboxComponent *hitbox) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    entity[id].flag &= ~COLLISION_DETECTED;
  }

  for (int idA = 0; idA < MAX_ENTITIES; idA++) {
    if (entity[idA].flag & DETECTS_COLLISIONS) {
      for (int idB = idA + 1; idB < MAX_ENTITIES; idB++) {
        if (entity[idB].flag & TRIGGERS_COLLISIONS) {
          if (checkForCollision(&position[idA], &velocity[idA], &hitbox[idA],
                                &position[idB], &velocity[idB], &hitbox[idB])) {
            entity[idA].flag |= COLLISION_DETECTED;
            entity[idB].flag |= COLLISION_DETECTED;
          }
        }
      }
    }
  }
}

void updateMovementSystem(Entity *entity, PositionComponent *position,
                          VelocityComponent *velocity) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (entity[id].flag & VELOCITY_COMPONENT) {

      if ((entity[id].flag & COLLISION_DETECTED)) {
        velocity->dx = 0;
        velocity->dy = 0;
      } else {
        position->x += velocity->dx;
        position->y += velocity->dy;
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
