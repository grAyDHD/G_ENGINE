#include "../includes/ECS.h"
#include "../includes/characterAnimator.h" // later update to pull from library
#include "Bedroom.h"
#include "input/in.h"

void patrolBehavior(EntitySystem *system, int entityId) {
  // this should be a walking back and forth animagion.
  AIComponent *ai = &system->world->ai[entityId];
  PositionComponent *position = &system->world->position[entityId];
  AnimationComponent *animation = &system->world->animation[entityId];
  // p2 = distance
  ai->param2 = 20;

  if (animation->direction == LEFT) {
    position->x--;
    ai->param1++;
    if (ai->param1 >= ai->param2) {
      ai->param1 = 0;
      animation->direction = RIGHT;
    }
  } else { // RIGHT
    position->x++;
    ai->param1++;
    if (ai->param1 >= ai->param2) {
      ai->param1 = 0;
      animation->direction = LEFT;
    }
  }

  animation->keyframe++;
  if (animation->keyframe >= animation->keyframeInterval) {
    animation->keyframe = 0;
    animation->frameNumber++;
    if (animation->frameNumber == 4) {
      animation->frameNumber = 0;
    }
  }
};

void updateBehaviorSystem(EntitySystem *system, ComponentManager *world) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (system->entities[i].componentMask & COMPONENT_AI) {
      system->world->ai[i].aiBehavior(system, i);
    }
  }
};

void updateRenderSystem(EntitySystem *system, ComponentManager *world) {
  for (int i = 0; i < system->maxEntities; i++) {
    if (system->entities[i].componentMask & COMPONENT_ANIMATION) {
      renderEntity(system, i);
    }
  }
};

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

int createNPC(EntitySystem *system, const void *spriteSheet) {
  int npcId = createEntity(system, NPC_ENTITY);

  system->world->animation[npcId] = (AnimationComponent){.frameNumber = 0,
                                                         .direction = RIGHT,
                                                         .state = WALK,
                                                         .keyframe = 0,
                                                         .keyframeInterval = 8};
  system->world->sprite[npcId] = (SpriteComponent){.spriteSheet = spriteSheet};
  system->world->ai[npcId].aiBehavior = patrolBehavior;

  return npcId;
}

int createPlayer(EntitySystem *system, const void *spriteSheet) {
  int playerId = createEntity(system, PLAYER_ENTITY);

  system->world->animation[playerId] =
      (AnimationComponent){.frameNumber = 0,
                           .direction = LEFT,
                           .state = WALK,
                           .keyframe = 0,
                           .keyframeInterval = 8};
  system->world->sprite[playerId] =
      (SpriteComponent){.spriteSheet = spriteSheet};

  system->world->input[playerId].handleInput = playerInputHandler;

  system->inputEntities[system->inputEntityCount++] = playerId;
  return playerId;
}

void inline renderEntity(EntitySystem *system, int playerId) {
  SpriteFrame32Bit(&system->world->position[playerId],
                   &system->world->animation[playerId],
                   system->world->sprite[playerId].spriteSheet);
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

void playerInputHandler(EntitySystem *system, int entityId) {
  AnimationComponent *animation = &system->world->animation[entityId];

  if (keyDown(U)) {
    system->world->position[entityId].y -= 1;
    system->world->animation[entityId].direction = UP;
    animation->state = WALK;
  } else if (keyDown(D)) {
    system->world->position[entityId].y += 1;
    system->world->animation[entityId].direction = DOWN;
    animation->state = WALK;
  } else if (keyDown(L)) {
    system->world->position[entityId].x -= 1;
    system->world->animation[entityId].direction = LEFT;
    animation->state = WALK;
  } else if (keyDown(R)) {
    system->world->position[entityId].x += 1;
    system->world->animation[entityId].direction = RIGHT;
    animation->state = WALK;
  }

  if (keyReleased(U | D | L | R)) {
    animation->frameNumber = 0;
    animation->state = IDLE;
    animation->keyframe = 0;
  }

  animation->keyframe++;
  if (animation->keyframe >= animation->keyframeInterval) {
    animation->keyframe = 0;
    animation->frameNumber++;
    if (animation->frameNumber == 4) {
      animation->frameNumber = 0;
    }
  }
}
