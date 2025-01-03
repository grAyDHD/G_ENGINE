#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"
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
