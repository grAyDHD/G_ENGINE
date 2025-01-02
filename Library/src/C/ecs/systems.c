#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"

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
