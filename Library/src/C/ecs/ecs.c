#include "../../include/ecs/ecs.h"
#include "entities.c"
#include "graphics/m3Text.h"

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

void inline renderEntity(ECS *ecs, int entityId) {
  SpriteFrame32Bit(&ecs->components->position[entityId],
                   &ecs->components->animation[entityId],
                   ecs->components->sprite[entityId].spriteSheet);
}

void inline renderEntityText(ECS *ecs, int entityId) {
  int x = ecs->components->position[entityId].x;
  int y = ecs->components->position[entityId].y;
  gprintf(x, y, ecs->components->text[entityId].text, 0);
}
// todo: don't hardcode entities
void inline clearSpriteFrames(ECS *system, ComponentStorage *world,
                              const void *image) {
  clearSpriteFrame(world->position[0].x, world->position[0].y, 32, image);
  clearSpriteFrame(world->position[1].x, world->position[1].y, 32, image);
}
