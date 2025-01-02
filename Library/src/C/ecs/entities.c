#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"

int createEntity(ECS *ecs, int flag) {
  if (ecs->nextEntityId >= MAX_ENTITIES) {
    return -1;
  }

  Entity *entity = &ecs->entity[ecs->nextEntityId];
  entity->ID = ecs->nextEntityId;
  entity->flag = flag;

  if (flag & COMPONENT_POSITION) {
    ecs->components->position[ecs->nextEntityId] =
        (PositionComponent){120, 120};
  }

  if (flag & COMPONENT_VELOCITY) {
    ecs->components->velocity[ecs->nextEntityId] = (VelocityComponent){0, 0};
  }

  return ecs->nextEntityId++;
}

int createNPC(ECS *ecs, const void *spriteSheet) {
  int npcId = createEntity(ecs, NPC_ENTITY);

  ecs->components->animation[npcId] =
      (AnimationComponent){.frameNumber = 0,
                           .direction = RIGHT,
                           .state = WALK,
                           .keyframe = 0,
                           .keyframeInterval = 8};
  ecs->components->sprite[npcId] =
      (SpriteComponent){.spriteSheet = spriteSheet};
  ecs->components->ai[npcId].aiBehavior = patrolBehavior;

  return npcId;
}

int createPlayer(ECS *ecs, const void *spriteSheet) {
  int playerId = createEntity(ecs, PLAYER_ENTITY);

  ecs->components->animation[playerId] =
      (AnimationComponent){.frameNumber = 0,
                           .direction = LEFT,
                           .state = WALK,
                           .keyframe = 0,
                           .keyframeInterval = 8};
  ecs->components->sprite[playerId] =
      (SpriteComponent){.spriteSheet = spriteSheet};

  ecs->components->input[playerId].handleInput = playerInputHandler;
  //  ecs->inputEntities[ecs->inputEntityCount++] = playerId;
  //  instead, iterate through id array, set if > -1
  //  else, reintroduce inputEntityCount
  return playerId;
}
