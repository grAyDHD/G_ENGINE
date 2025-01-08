#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "components.h"
#include "entities.h"

#define MAX_ENTITIES 10
#define MAX_INPUT_ENTITIES 4

typedef struct ECS {
  ComponentStorage *components;
  Entity entity[MAX_ENTITIES]; // each entity is a struct of id and flags
  int inputEntityId[MAX_INPUT_ENTITIES];
  int nextEntityId;
  int entityCount;
  int inputEntityCount;
} ECS;

void updatePhysicsSystem(Entity *entity, VelocityComponent *velocity,
                         AccelerationComponent *acceleration);

void updateBehaviorSystem(Entity *entity, AIComponent *ai);

void updateInputSystem(Entity *entity, InputComponent *input);

void updateCollisionSystem(Entity *entity, PositionComponent *position,
                           VelocityComponent *velocity,
                           HitboxComponent *hitbox);

void updateAnimationSystem(Entity *entity, AnimationComponent *animation);

void updateRenderSystem(Entity *entity, AnimationComponent *animation,
                        DrawingComponent *draw);

#endif // !SYSTEMS_H

/*

typedef struct ECS ECS;
typedef struct ComponentStorage ComponentStorage;

void updateInputSystem(ECS *ecs, ComponentStorage *components);
void updateBehaviorSystem(ECS *ecs, ComponentStorage *components);
void updatePhysicsSystem(ECS *ecs, ComponentStorage *components);
void updateRenderSystem(ECS *ecs, ComponentStorage *components);
void updateCollisionSystem(ECS *ecs, ComponentStorage *components);
// void updateMovementSystem(ECS *ecs, ComponentStorage *components);
void updateMovementSystem(Entity *entity, PositionComponent *position,
                          VelocityComponent *velocity);
void updateAnimationSystem(ECS *ecs, ComponentStorage *components);
*/
