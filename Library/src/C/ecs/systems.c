#include "ecs/systems.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"
#include "ecs/components.h"
#include "ecs/entities.h"

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
  // hard coded for player, make sure player is first created entity
  // will refine logic once other input entities are necessary.  Below code
  // stopped working after refactor unsure why.
  /*
  for (int i = 0; i < ecs->inputEntityCount; i++) {
    int entityId = ecs->inputEntityId[i];
    if (ecs->entity[i].flag & ENABLE_INPUT) {
      components->input[entityId].handleInput(ecs, entityId);
    }
  }
  */
}

static int gravityDirection = 1;

void updateGravitySystem(ECS *ecs, ComponentStorage *components) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (ecs->entity[id].flag & ENABLE_PHYSICS) {
      AccelerationComponent *acc = &components->acceleration[id];
      acc->ay = GRAVITY * gravityDirection;
    }
  }
}

void updateMovementSystem(ECS *ecs, ComponentStorage *components) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (ecs->entity[id].flag & COMPONENT_VELOCITY) {
      VelocityComponent *vel = &components->velocity[id];
      PositionComponent *pos = &components->position[id];
      pos->x += vel->dx;
      pos->y += vel->dy;
    }
  }
}

void updateRenderSystem(ECS *ecs, ComponentStorage *components) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (ecs->entity[i].flag & COMPONENT_ANIMATION) {
      renderEntity(ecs, i);
    }
  }
};

void updatePhysicsSystem(ECS *ecs, ComponentStorage *components) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (ecs->entity[id].flag & ENABLE_PHYSICS)
      continue;

    VelocityComponent *vel = &components->velocity[id];
    AccelerationComponent *acc = &components->acceleration[id];
    PositionComponent *pos = &components->position[id];

    vel->dx += acc->ax;
    vel->dy += acc->ay;

    pos->x += vel->dx;
    pos->y += vel->dy;
  }
}

void handleCollisionSystem(ECS *ecs, ComponentStorage *components) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (!(ecs->entity[id].flag & ENABLE_PHYSICS))
      continue;

    PositionComponent *pos = &components->position[id];
    VelocityComponent *vel = &components->velocity[id];
    HitboxComponent *hitbox = &components->hitbox[id];

    if (gravityDirection == 1) {
      if (pos->y >= SH - hitbox->height) {
        pos->y = SH - hitbox->height;
        vel->dy = -(vel->dy / 2);
        ecs->entity[id].flag |= ON_GROUND;
      } else if (pos->y <= 0) {
        vel->dy = 0;
        ecs->entity[id].flag &= ~ON_GROUND;
      } else {
        ecs->entity[id].flag &= ~ON_GROUND;
      }
    } else if (gravityDirection == -1) {
      if (pos->y <= 0) {
        pos->y = 0;
        vel->dy = -(vel->dy / 2);
        ecs->entity[id].flag |= ON_GROUND;
      } else if (pos->y >= SH - hitbox->height) {
        pos->y = SH - hitbox->height;
        vel->dy = 0;
        ecs->entity[id].flag &= ~ON_GROUND;
      } else {
        ecs->entity[id].flag &= ~ON_GROUND;
      }
    }

    // Handle horizontal collisions
    if (pos->x < 0) {
      pos->x = 0;
      vel->dx = 0;
    }
    if (pos->x >= SW - hitbox->width) {
      pos->x = SW - hitbox->width;
      vel->dx = 0;
    }
  }
}

void physicsInputHandler(ECS *ecs, int entityId) {
  VelocityComponent *vel = &ecs->components->velocity[entityId];
  AccelerationComponent *acc = &ecs->components->acceleration[entityId];

  if (keyDown(A) && (ecs->entity[entityId].flag & ON_GROUND)) {
    vel->dy = -SHOOT_VEL * gravityDirection;
  }

  if (keyDown(B)) {
    gravityDirection = -1;
  } else {
    gravityDirection = 1;
  }

  if (keyDown(SL)) {
    vel->dx = 0;
    vel->dy = 0;
    acc->ax = 0;
    acc->ay = 0;
  } else {
    if (keyDown(L)) {
      vel->dx = -MOVE_SPEED;
    } else if (keyDown(R)) {
      vel->dx = MOVE_SPEED;
    } else {
      vel->dx = 0;
    }

    if (keyDown(LT)) {
      vel->dx = -SHOOT_VEL;
      vel->dy = -SHOOT_VEL;
    } else if (keyDown(RT)) {
      vel->dx = SHOOT_VEL;
      vel->dy = -SHOOT_VEL;
    }
  }
}
