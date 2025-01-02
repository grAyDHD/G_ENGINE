#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"

void updateBehaviorSystem(ECS *ecs, ComponentStorage *components) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (ecs->entity[i].flag & COMPONENT_AI) {
      ecs->components->ai[i].aiBehavior(ecs, i);
    }
  }
};

void updateRenderSystem(ECS *ecs, ComponentStorage *components) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (ecs->entity[i].flag & COMPONENT_ANIMATION) {
      renderEntity(ecs, i);
    }
  }
};

void updateInputSystem(ECS *ecs, ComponentStorage *components) {
  updateKeys();
  for (int i = 0; i < MAX_INPUT_ENTITIES; i++) {
    int entityId = ecs->inputEntityId[i];
    if (ecs->entity[i].flag & ENABLE_INPUT) {
      components->input[entityId].handleInput(ecs, entityId);
    }
  }
}
