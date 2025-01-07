#include "ecs/systems.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"
#include "ecs/components.h"
#include "ecs/entities.h"

static int gravityDirection = 1;

void updateBehaviorSystem(ECS *ecs, ComponentStorage *components) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (ecs->entity[i].flag & AI_COMPONENT) {
      ecs->components->ai[i].aiBehavior(ecs, i);
    }
  }
};

void updateInputSystem(ECS *ecs, ComponentStorage *components) {
  updateKeys();
  components->input[0].handleInput(ecs, 0);
}

void updateRenderSystem(ECS *ecs, ComponentStorage *components) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (ecs->entity[id].flag & ANIMATION_COMPONENT) {
      renderEntity(ecs, id);
    }

    if (ecs->entity[id].flag & DRAWING_COMPONENT) {
      components->draw[id].drawingRoutine(ecs, id);
    }
  }
};

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

void updateCollisionSystem(ECS *ecs, ComponentStorage *components) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    ecs->entity[id].flag &= ~COLLISION_DETECTED;
  }

  for (int idA = 0; idA < MAX_ENTITIES; idA++) {
    if (ecs->entity[idA].flag & DETECTS_COLLISIONS) {
      for (int idB = idA + 1; idB < MAX_ENTITIES; idB++) {
        if (ecs->entity[idB].flag & TRIGGERS_COLLISIONS) {
          if (checkForCollision(
                  &components->position[idA], &components->velocity[idA],
                  &components->hitbox[idA], &components->position[idB],
                  &components->velocity[idB], &components->hitbox[idB])) {
            ecs->entity[idA].flag |= COLLISION_DETECTED;
            ecs->entity[idB].flag |= COLLISION_DETECTED;
          }
        }
      }
    }
  }
}

void updateMovementSystem(ECS *ecs, ComponentStorage *components) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (ecs->entity[id].flag & VELOCITY_COMPONENT) {
      VelocityComponent *velocity = &components->velocity[id];
      PositionComponent *position = &components->position[id];

      if ((ecs->entity[id].flag & COLLISION_DETECTED)) {
        velocity->dx = 0;
        velocity->dy = 0;
      } else {
        position->x += velocity->dx;
        position->y += velocity->dy;
      }
    }
  }
}

void updateAnimationSystem(ECS *ecs, ComponentStorage *components) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (ecs->entity[id].flag & ANIMATION_COMPONENT) {
      AnimationComponent *animation = &components->animation[id];

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
