#ifndef ENTITIES_H
#define ENTITIES_H

// todo: separate component flags and state flags
// maybe add type flag?  npc, menu item, etc.
typedef struct {
  int ID;
  int flag;
} Entity;


#define POSITION_COMPONENT (1 << 0)
#define VELOCITY_COMPONENT (1 << 1)
#define ACCELERATION_COMPONENT (1 << 2)
#define ANIMATION_COMPONENT (1 << 3)
#define SPRITE_COMPONENT (1 << 4)
#define INPUT_COMPONENT (1 << 5)
#define AI_COMPONENT (1 << 6)
#define HITBOX_COMPONENT (1 << 7)
#define DRAWING_COMPONENT (1 << 8)
#define PREVIOUS_POSITION_COMPONENT (1 << 9)
#define TEXT_COMPONENT (1 << 10)

// TIMER_COMPONENT
// sound component?

#define DIRTY (1 << 14)
#define SPRITE_FLAG (1 << 15)
#define ENABLE_INPUT (1 << 16)
#define ENABLE_GRAVITY (1 << 17)
#define ON_GROUND (1 << 18)
#define TRIGGERS_COLLISIONS (1 << 19)
#define DETECTS_COLLISIONS (1 << 20)
// #define COLLISION_DETECTED (1 << 21)
#define PHYSICS_FLAG (1 << 22)
#define STATIC_COLLIDER (1 << 23)
#define IS_GROUND (1 << 24)
#define ACTIVE (1 << 25)
#define GAME_ENTITY (1 << 26)
#define PAUSE_UI (1 << 27)
#define HUD_UI (1 << 28)
#define MENU_UI (1 << 29)

#define HORIZONTAL_COLLISION (1 << 30)
#define VERTICAL_COLLISION (1 << 31)

// #define PLAYER_FLAG (1 << 19)
//  define PLAYER_ENTITY ENEMY_ENTITY INTERACTABLE_ENTITY

// todo: does player need acceleration or previous position? check physics demo as well for comparison
#define PLAYER_ENTITY                                                          \
  (POSITION_COMPONENT | PREVIOUS_POSITION_COMPONENT | VELOCITY_COMPONENT | ANIMATION_COMPONENT |             \
   SPRITE_COMPONENT | INPUT_COMPONENT | HITBOX_COMPONENT | PHYSICS_FLAG | \
   DETECTS_COLLISIONS | TRIGGERS_COLLISIONS | SPRITE_FLAG | GAME_ENTITY | ACTIVE | DIRTY)

#define NPC_ENTITY                                                             \
  (POSITION_COMPONENT | VELOCITY_COMPONENT | ANIMATION_COMPONENT |             \
   SPRITE_COMPONENT | AI_COMPONENT | HITBOX_COMPONENT | DETECTS_COLLISIONS |   \
   TRIGGERS_COLLISIONS | SPRITE_FLAG | DIRTY)

#define BOUNDARY_ENTITY                                                        \
  (POSITION_COMPONENT | HITBOX_COMPONENT | TRIGGERS_COLLISIONS |               \
   STATIC_COLLIDER | GAME_ENTITY | ACTIVE)

#endif // !ENTITIES_H
