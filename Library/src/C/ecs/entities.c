#include "ecs/entities.h"
#include "../../include/ecs/ecs.h"
#include "ecs/components.h"
#include "graphics/video.h"
#include "math/math.h"

/*
if (flag & COMPONENT_INPUT) {
  ecs->inputEntityCount++;
}
*/

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

  return ecs->nextEntityId++;
}

int createPlayer(ECS *ecs, const void *spriteSheet) {
  int playerId = createEntity(ecs, PLAYER_ENTITY);

  ecs->components->position[playerId] =
      // this should be provided as an argument
      (PositionComponent){.x = 80, .y = 80};

  ecs->components->hitbox[playerId] =
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
      (PositionComponent){.x = 120, .y = 100};

  ecs->components->hitbox[npcId] = (HitboxComponent){.width = 32, .height = 32};
  ecs->components->animation[npcId] =
      (AnimationComponent){.frameNumber = 0,
                           .direction = RIGHT,
                           .state = WALK,
                           .keyframe = 0,
                           .keyframeInterval = 8};
  ecs->components->sprite[npcId] =
      (SpriteComponent){.spriteSheet = spriteSheet};

  // NPC BEHAVIOR, make assignable, assignBehavior function?  states and
  // triggers to assign specific behavior under certain conditions
  ecs->components->ai[npcId].aiBehavior = patrolBehavior;

  return npcId;
}

void createScreenBorders(ECS *ecs) {
  // BOUNDARY_ENTITY = bitmask for:
  //  POSITION_COMPONENT| HITBOX_COMPONENT | TRIGGERS_COLLISIONS

  /*---Create Left Border---*/
  int boundaryId = createEntity(ecs, BOUNDARY_ENTITY);
  ecs->components->position[boundaryId] = (PositionComponent){.x = 0, .y = 0};
  ecs->components->hitbox[boundaryId] =
      (HitboxComponent){.width = 0, .height = SH};

  /*---Create Right Border---*/
  boundaryId = createEntity(ecs, BOUNDARY_ENTITY);
  ecs->components->position[boundaryId] =
      (PositionComponent){.x = INT_TO_FIXED(SW), .y = 0};
  ecs->components->hitbox[boundaryId] =
      (HitboxComponent){.width = 0, .height = SH};

  /*---Create Bottom Border---*/
  boundaryId = createEntity(ecs, BOUNDARY_ENTITY);
  ecs->components->position[boundaryId] =
      (PositionComponent){.x = 0, .y = INT_TO_FIXED(SH)};
  ecs->components->hitbox[boundaryId] =
      (HitboxComponent){.width = SW, .height = 0};

  /*---Create Top Border---*/
  boundaryId = createEntity(ecs, BOUNDARY_ENTITY);
  ecs->components->position[boundaryId] = (PositionComponent){.x = 0, .y = 0};
  ecs->components->hitbox[boundaryId] =
      (HitboxComponent){.width = SW, .height = 0};
}
