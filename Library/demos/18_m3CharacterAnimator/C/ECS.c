#include "../includes/ECS.h"
#include "../includes/characterAnimator.h" // later update to pull from library
#include "Bedroom.h"
#include "input/in.h"

int initEntitySystem(EntitySystem *system, ComponentManager *world) {
  system->world = world;
  system->nextEntityId = 0;
  system->maxEntities = MAX_ENTITIES;
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

void playerInputHandler(EntitySystem *system, int entityId) {
  if (keyDown(U)) {
    system->world->position[entityId].y -= 1;
    system->world->animation[entityId].state = WALK;
    system->world->animation[entityId].direction = UP;
  } else if (keyDown(D)) {
    system->world->position[entityId].y += 1;
    system->world->animation[entityId].state = WALK;
    system->world->animation[entityId].direction = DOWN;
  } else if (keyDown(L)) {
    system->world->position[entityId].x -= 1;
    system->world->animation[entityId].state = WALK;
    system->world->animation[entityId].direction = LEFT;
  } else if (keyDown(R)) {
    system->world->position[entityId].x += 1;
    system->world->animation[entityId].state = WALK;
    system->world->animation[entityId].direction = RIGHT;
  }
}

int createPlayer(EntitySystem *system, const void *spriteSheet) {
  int playerId = createEntity(system, PLAYER_ENTITY);

  system->world->animation[playerId] =
      (AnimationComponent){.frameNumber = 1, .direction = LEFT, .state = WALK};
  system->world->sprite[playerId] =
      (SpriteComponent){.spriteSheet = spriteSheet};

  system->world->input[playerId].handleInput = playerInputHandler;

  system->inputEntities[system->inputEntityCount++] = playerId;
  return playerId;
}

void inline renderPlayer(EntitySystem *system, int playerId) {
  SpriteFrame32Bit(&system->world->position[playerId],
                   &system->world->animation[playerId],
                   system->world->sprite[playerId].spriteSheet);
  restoreFrameBackground(system->world->position[playerId].x,
                         system->world->position[playerId].y, 32,
                         BedroomBitmap);
}

void updateInputSystem(EntitySystem *system, ComponentManager *world) {
  updateKeys();
  for (int i = 0; i < system->inputEntityCount; i++) {
    int entityId = system->inputEntities[i];

    if (system->entities[i].componentMask & ENABLE_INPUT) {
      world->input[entityId].handleInput(system, entityId);
    }
  }
}

/*
    updateKeys();

  handlePlayerInput() {
    */

//   if (keyReleased(U) | keyReleased(D) | keyReleased(L) | keyReleased(R))
//   {
//    world.animation[playerId].frameNumber = 0;
//    world.animation[playerId].state = IDLE;
// }
