#ifndef ECS_H
#define ECS_H

#include "components.h"
#include "entities.h"
#include "systems.h"

#define MAX_ENTITIES 10
#define MAX_INPUT_ENTITIES 4

struct ComponentStorage {
  PositionComponent position[MAX_ENTITIES];
  VelocityComponent velocity[MAX_ENTITIES];
  AnimationComponent animation[MAX_ENTITIES];
  SpriteComponent sprite[MAX_ENTITIES];
  InputComponent input[MAX_INPUT_ENTITIES];
  AIComponent ai[MAX_ENTITIES];
};

struct ECS {
  ComponentStorage *components;
  Entity entity[MAX_ENTITIES]; // each entity is a struct of id and flags
  int inputEntityId[MAX_INPUT_ENTITIES];
  int nextEntityId;
  int entityCount;
};

void renderEntity(ECS *ecs, int entityId);
int initEntitySystem(ECS *ecs, ComponentStorage *world);

#endif // ECS_H
