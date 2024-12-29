#include "../includes/ECS.h"

int initEntitySystem(EntitySystem *system, ComponentManager *world,
                     Entity *entities, int maxEntities) {
  system->world = world;
  system->entities = entities;
  system->nextEntityId = 0;
  system->maxEntities = maxEntities;
  return 0;
}

int createEntity(EntitySystem *system, int componentMask) {
  if (system->nextEntityId >= system->maxEntities) {
    return -1;
  }

  Entity *entity = &system->entities[system->nextEntityId];
  entity->entityID = system->nextEntityId;
  entity->componentMask = componentMask;

  if (componentMask & COMPONENT_POSITION) {
    system->world->position[system->nextEntityId] =
        (PositionComponent){120, 120};
  }

  if (componentMask & COMPONENT_VELOCITY) {
    system->world->velocity[system->nextEntityId] = (VelocityComponent){0, 0};
  }

  return system->nextEntityId++;
}

int createPlayer(EntitySystem *system, const void *spriteSheet) {
  int playerId = createEntity(system, PLAYER_ENTITY);

  system->world->animation[playerId] =
      (AnimationComponent){.frameNumber = 1, .direction = LEFT, .state = WALK};
  system->world->sprite[playerId] =
      (SpriteComponent){.spriteSheet = spriteSheet};

  return playerId;
}

void renderPlayer(EntitySystem *system, int playerId) {
  SpriteFrame32Bit(&system->world->position[playerId],
                   &system->world->animation[playerId],
                   system->world->sprite[playerId].spriteSheet);
}

/*
int createEntity(int componentMask) {
  if (nextEntityId >= MAX_ENTITIES) {
    return -1;
  }

  Entity *entity = &entities[nextEntityId];
  entity->entityID = nextEntityId;
  entity->componentMask = componentMask;

  if (componentMask & COMPONENT_POSITION) {
    world.position[nextEntityId] = (PositionComponent){120, 120};
  }

  if (componentMask & COMPONENT_VELOCITY) {
    world.velocity[nextEntityId] = (VelocityComponent){0, 0};
  }

  return nextEntityId++;
}

int createPlayer(const void *spriteSheet) {

  int playerId = createEntity(PLAYER_ENTITY);

  world.animation[playerId] =
      (AnimationComponent){.frameNumber = 1, .direction = LEFT, .state = WALK};
  world.sprite[playerId] = (SpriteComponent){.spriteSheet = spriteSheet};

  return playerId;
}

void renderPlayer(int playerId, ComponentManager *world) {
  SpriteFrame32Bit(&world->position[playerId], &world->animation[playerId],
                   world->sprite[playerId].spriteSheet);
}
*/
