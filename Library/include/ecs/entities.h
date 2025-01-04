#ifndef ENTITIES_H
#define ENTITIES_H
#include "systems.h"

typedef struct {
  int ID;
  int flag;
} Entity;

#define COMPONENT_POSITION (1 << 0)
#define COMPONENT_VELOCITY (1 << 1)
#define ACCELERATION_COMPONENT (1 << 2)
#define COMPONENT_ANIMATION (1 << 3)
#define COMPONENT_SPRITE (1 << 4)
#define COMPONENT_INPUT (1 << 5)
#define COMPONENT_AI (1 << 6)
#define COMPONENT_HITBOX (1 << 7)

#define ENABLE_INPUT (1 << 16)
#define ENABLE_PHYSICS (1 << 17)
#define ON_GROUND (1 << 18)
#define ENABLE_COLLISIONS (1 << 19)
#define ENABLE_COLLISION_DETECTION (1 << 20)
#define COLLISION_DETECTED (1 << 21)
// #define PLAYER_FLAG (1 << 19)
//  define PLAYER_ENTITY ENEMY_ENTITY INTERACTABLE_ENTITY

#define PLAYER_ENTITY                                                          \
  (COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_ANIMATION |             \
   COMPONENT_SPRITE | COMPONENT_INPUT | COMPONENT_HITBOX | ENABLE_INPUT |      \
   ENABLE_COLLISIONS)

#define NPC_ENTITY                                                             \
  (COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_ANIMATION |             \
   COMPONENT_SPRITE | COMPONENT_AI | COMPONENT_HITBOX | ENABLE_COLLISIONS)

int createEntity(ECS *ecs, int activeComponentMask);
int createPlayer(ECS *ecs, const void *spriteSheet);
int createNPC(ECS *ecs, const void *spriteSheet);

#endif // !ENTITIES_H
