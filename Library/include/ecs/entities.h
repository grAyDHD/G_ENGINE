#ifndef ENTITIES_H
#define ENTITIES_H
#include "systems.h"

typedef struct {
  int ID;
  int flag;
} Entity;

#define COMPONENT_POSITION (1 << 0)
#define COMPONENT_VELOCITY (1 << 1)
#define COMPONENT_ANIMATION (1 << 2)
#define COMPONENT_SPRITE (1 << 3)
#define COMPONENT_INPUT (1 << 4)
#define COMPONENT_AI (1 << 5)

#define ENABLE_INPUT (1 << 16)

// #define PLAYER_FLAG (1 << 17)
//  define PLAYER_ENTITY ENEMY_ENTITY INTERACTABLE_ENTITY
#define PLAYER_ENTITY                                                          \
  (COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_ANIMATION |             \
   COMPONENT_SPRITE | COMPONENT_INPUT | ENABLE_INPUT)

#define NPC_ENTITY                                                             \
  (COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_ANIMATION |             \
   COMPONENT_SPRITE | COMPONENT_AI)

int createEntity(ECS *ecs, int activeComponentMask);
int createPlayer(ECS *ecs, const void *spriteSheet);
int createNPC(ECS *ecs, const void *spriteSheet);

#endif // !ENTITIES_H
