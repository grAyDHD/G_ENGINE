#include "ecs/entities.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"
#include "ecs/components.h"

int createEntity(ECS *ecs, int flag) {
  if (ecs->nextEntityId >= MAX_ENTITIES) {
    return -1;
  }

  Entity *entity = &ecs->entity[ecs->nextEntityId];
  entity->ID = ecs->nextEntityId;
  entity->flag = flag;

  // this should be zero unless
  if (flag & VELOCITY_COMPONENT) {
    ecs->components->velocity[ecs->nextEntityId] =
        (VelocityComponent){.dx = 0, .dy = 0};
  }

  /*
  if (flag & COMPONENT_INPUT) {
    ecs->inputEntityCount++;
  }
*/

  return ecs->nextEntityId++;
}

int createPlayer(ECS *ecs, const void *spriteSheet) {
  int playerId = createEntity(ecs, PLAYER_ENTITY);

  ecs->components->position[playerId] =
      // this should be provided as an argument
      (PositionComponent){.x = 80, .y = 80};

  ecs->components->hitbox[playerId] =
      // not positive how this shall be implemented.  look up table based on
      // sprite sheet probably
      (HitboxComponent){.width = 32, .height = 32};

  ecs->components->animation[playerId] =
      (AnimationComponent){.frameNumber = 0,
                           .direction = LEFT,
                           .state = WALK,
                           .keyframe = 0,
                           .keyframeInterval = 8};

  ecs->components->sprite[playerId] =
      (SpriteComponent){.spriteSheet = spriteSheet};

  ecs->components->input[playerId].handleInput = playerInputHandler;
  ecs->inputEntityId[ecs->inputEntityCount++] = playerId;
  //  instead, iterate through id array, set if > -1
  //  else, reintroduce inputEntityCount
  return playerId;
}

int createNPC(ECS *ecs, const void *spriteSheet) {
  int npcId = createEntity(ecs, NPC_ENTITY);

  ecs->components->position[ecs->nextEntityId] =
      // this should be provided as an argument
      (PositionComponent){.x = 120, .y = 120};

  ecs->components->hitbox[npcId] = (HitboxComponent){.width = 32, .height = 32};
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
