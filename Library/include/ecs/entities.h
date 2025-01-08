#ifndef ENTITIES_H
#define ENTITIES_H

typedef struct {
  int ID;
  int flag;
} Entity;

// position velocity acceleration

#define POSITION_COMPONENT (1 << 0)
#define VELOCITY_COMPONENT (1 << 1)
#define ACCELERATION_COMPONENT (1 << 2)
#define ANIMATION_COMPONENT (1 << 3)
#define SPRITE_COMPONENT (1 << 4)
#define INPUT_COMPONENT (1 << 5)
#define AI_COMPONENT (1 << 6)
#define HITBOX_COMPONENT (1 << 7)
#define DRAWING_COMPONENT (1 << 8)
// TIMER_COMPONENT
// sound component, direct sound component?

#define ENABLE_INPUT (1 << 16)
#define ENABLE_PHYSICS (1 << 17)
#define ON_GROUND (1 << 18)
#define TRIGGERS_COLLISIONS (1 << 19)
#define DETECTS_COLLISIONS (1 << 20)
#define COLLISION_DETECTED (1 << 21)
#define PHYSICS_FLAG (1 << 22)

// #define PLAYER_FLAG (1 << 19)
//  define PLAYER_ENTITY ENEMY_ENTITY INTERACTABLE_ENTITY

#define PLAYER_ENTITY                                                          \
  (POSITION_COMPONENT | VELOCITY_COMPONENT | ANIMATION_COMPONENT |             \
   SPRITE_COMPONENT | INPUT_COMPONENT | HITBOX_COMPONENT | ENABLE_INPUT |      \
   DETECTS_COLLISIONS | TRIGGERS_COLLISIONS)

#define NPC_ENTITY                                                             \
  (POSITION_COMPONENT | VELOCITY_COMPONENT | ANIMATION_COMPONENT |             \
   SPRITE_COMPONENT | AI_COMPONENT | HITBOX_COMPONENT | DETECTS_COLLISIONS |   \
   TRIGGERS_COLLISIONS)

#define BOUNDARY_ENTITY                                                        \
  (POSITION_COMPONENT | VELOCITY_COMPONENT | HITBOX_COMPONENT |                \
   TRIGGERS_COLLISIONS)

#endif // !ENTITIES_H
