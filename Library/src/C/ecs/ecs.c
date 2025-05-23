#include "../../include/ecs/ecs.h"
#include "entities.c"

int initEntitySystem(ECS *ecs, ComponentStorage *components) {
  if (!ecs || !components) {
    return -1;
  }

  ecs->entityCount = 0;
  ecs->nextEntityId = 0;
  ecs->components = components;

  // Initialize Entity array
  for (int i = 0; i < MAX_ENTITIES; i++) {
    ecs->entity[i].ID = -1;  // Mark as unused
    ecs->entity[i].flag = 0; // No components attached
  }

  // Initialize Input Entity ID array
  for (int i = 0; i < MAX_INPUT_ENTITIES; i++) {
    ecs->inputEntityId[i] = -1; // Mark as unused
  }

  return 0;
}

void inline renderEntity(ECS *ecs, int playerId) {
  SpriteFrame32Bit(&ecs->components->position[playerId],
                   &ecs->components->animation[playerId],
                   ecs->components->sprite[playerId].spriteSheet);
}

void inline clearSpriteFrames(ECS *system, ComponentStorage *world,
                              const void *image) {
  clearSpriteFrame(world->position[0].x, world->position[0].y, 32, image);
  clearSpriteFrame(world->position[1].x, world->position[1].y, 32, image);
}
