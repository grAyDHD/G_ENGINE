#include "ecs/systems.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"
#include "ecs/components.h"
#include "ecs/entities.h"

static int gravityDirection = 1;

void updateBehaviorSystem(ECS *ecs, ComponentStorage *components) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (ecs->entity[i].flag & COMPONENT_AI) {
      ecs->components->ai[i].aiBehavior(ecs, i);
    }
  }
};

void updateInputSystem(ECS *ecs, ComponentStorage *components) {
  updateKeys();
  components->input[0].handleInput(ecs, 0);
}

void updateRenderSystem(ECS *ecs, ComponentStorage *components) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (ecs->entity[i].flag & COMPONENT_ANIMATION) {
      renderEntity(ecs, i);
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
    if (ecs->entity[idA].flag & ENABLE_COLLISIONS) {
      for (int idB = idA + 1; idB < MAX_ENTITIES; idB++) {
        if (ecs->entity[idB].flag & ENABLE_COLLISIONS) {
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
    if (ecs->entity[id].flag & COMPONENT_VELOCITY) {
      VelocityComponent *vel = &components->velocity[id];
      PositionComponent *pos = &components->position[id];

      if ((ecs->entity[id].flag & COLLISION_DETECTED)) {
        vel->dx = 0;
        vel->dy = 0;
      } else {
        pos->x += vel->dx;
        pos->y += vel->dy;
      }
    }
  }
}

/*

      // Try horizontal movement first
      pos->x += vel->dx;
      if (ecs->entity[id].flag & COLLISION_DETECTED) {
        pos->x -= vel->dx; // Undo horizontal movement if collision
      }

      // Try vertical movement second
      pos->y += vel->dy;
      if (ecs->entity[id].flag & COLLISION_DETECTED) {
        pos->y -= vel->dy; // Undo vertical movement if collision
      }
*/
