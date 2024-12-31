#ifndef ECS_H
#define ECS_H

#define MAX_ENTITIES 10
#define MAX_INPUT_ENTITIES 4

#define COMPONENT_PLAYER (1 << 0)
#define COMPONENT_POSITION (1 << 1)
#define COMPONENT_VELOCITY (1 << 2)
#define COMPONENT_ANIMATION (1 << 3)
#define COMPONENT_SPRITE (1 << 4)
#define COMPONENT_INPUT (1 << 5)

#define ENABLE_INPUT (1 << 16)

// define PLAYER_ENTITY ENEMY_ENTITY INTERACTABLE_ENTITY
#define PLAYER_ENTITY                                                          \
  (COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_ANIMATION |             \
   COMPONENT_SPRITE | COMPONENT_INPUT | ENABLE_INPUT)

typedef enum : int { DOWN = 0, UP, LEFT, RIGHT } DIRECTION;

typedef enum : int { IDLE = 0, WALK, TALK, RUN } STATE;

typedef struct {
  const void *spriteSheet; // pointer to active sprite 4 bytes
} SpriteComponent;

typedef struct {
  int frameNumber;
  DIRECTION direction;
  STATE state;
} AnimationComponent;

typedef struct {
  int x;
  int y;
} PositionComponent;

typedef struct {
  int dx;
  int dy;
} VelocityComponent;

typedef struct EntitySystem EntitySystem;

typedef struct {
  void (*handleInput)(EntitySystem *system, int entityId); // Custom behavior
} InputComponent;

typedef struct {
  PositionComponent position[MAX_ENTITIES];
  VelocityComponent velocity[MAX_ENTITIES];
  AnimationComponent animation[MAX_ENTITIES];
  SpriteComponent sprite[MAX_ENTITIES];
  InputComponent input[MAX_INPUT_ENTITIES];
} ComponentManager;

typedef struct {
  int entityID;
  int componentMask;
} Entity;

struct EntitySystem {
  ComponentManager *world;
  Entity entities[MAX_ENTITIES]; // each entity is a struct of id and flags
  int nextEntityId;
  int maxEntities;

  int inputEntities[MAX_INPUT_ENTITIES];
  int inputEntityCount;
};

int initEntitySystem(EntitySystem *system, ComponentManager *world);
int createEntity(EntitySystem *system, int componentMask);
int createPlayer(EntitySystem *system, const void *spriteSheet);
void playerInputHandler(EntitySystem *system, int entityId);
void renderPlayer(EntitySystem *system, int playerId);
void updateInputSystem(EntitySystem *system, ComponentManager *world);

#endif // ECS_H
