#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "components.h"
#include "entities.h"

typedef struct ECS {
  ComponentStorage *components;
  Entity entity[MAX_ENTITIES]; // each entity is a struct of id and flags
  int inputEntityId[MAX_INPUT_ENTITIES];
  int nextEntityId;
  int entityCount;
  int inputEntityCount;
} ECS;

void updateInputSystem(ECS *ecs, Entity *entity, InputComponent *input);
void updateBehaviorSystem(ECS *ecs, Entity *entity, AIComponent *ai);

void updatePhysicsSystem(Entity *entity, VelocityComponent *velocity,
                         AccelerationComponent *acceleration);
void updateCollisionSystem(Entity *entity, PositionComponent *position,
                           VelocityComponent *velocity,
                           HitboxComponent *hitbox);

void updateMovementSystem(Entity *entity, PositionComponent *position,
                          VelocityComponent *velocity);

void updateAnimationSystem(Entity *entity, AnimationComponent *animation);

void updateRenderSystem(ECS *ecs, Entity *entity, AnimationComponent *animation,
                        DrawingComponent *draw);

#endif // !SYSTEMS_H
