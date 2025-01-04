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

int checkForCollision(PositionComponent *position[2],
                      VelocityComponent *velocity[2],
                      HitboxComponent *hitbox[2]) {
  int Ax1 = position[0]->x + velocity[0]->dx;
  int Ax2 = Ax1 + hitbox[0]->width;
  int Ay1 = position[0]->y + velocity[0]->dy;
  int Ay2 = Ay1 + hitbox[0]->height;

  int Bx1 = position[1]->x;
  int Bx2 = Bx1 + hitbox[1]->width;
  int By1 = position[1]->y;
  int By2 = By1 + hitbox[1]->height;

  return (Bx2 > Ax1 && Bx1 < Ax2 && By2 > Ay1 && By1 < Ay2);
}

void updateCollisionSystem(ECS *ecs, ComponentStorage *components) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    ecs->entity[id].flag &= ~COLLISION_DETECTED; // Clear the collision flag
  }

  for (int idA = 0; idA < MAX_ENTITIES; idA++) {
    if (ecs->entity[idA].flag & ENABLE_COLLISIONS) {
      for (int idB = idA + 1; idB < MAX_ENTITIES; idB++) {
        if (ecs->entity[idB].flag & ENABLE_COLLISIONS) {

          VelocityComponent *velA = &components->velocity[idA];
          VelocityComponent *velB = &components->velocity[idB];
          PositionComponent *posA = &components->position[idA];
          PositionComponent *posB = &components->position[idB];
          HitboxComponent *hitA = &components->hitbox[idA];
          HitboxComponent *hitB = &components->hitbox[idB];

          VelocityComponent *velocity[2] = {velA, velB};
          PositionComponent *position[2] = {posA, posB};
          HitboxComponent *hitbox[2] = {hitA, hitB};

          if (checkForCollision(position, velocity, hitbox)) {
            ecs->entity[idA].flag |= COLLISION_DETECTED;
            ecs->entity[idB].flag |= COLLISION_DETECTED;
          };
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
