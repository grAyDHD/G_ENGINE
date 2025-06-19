#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "components.h"
#include "core/typedefs.h"
#include "entities.h"

typedef s32 fixed_s32;

typedef struct ECS {
  ComponentStorage *components;
  Entity entity[MAX_ENTITIES]; // each entity is a struct of id and flags
  int inputEntityId[MAX_INPUT_ENTITIES];
  int nextEntityId;
  int entityCount;
  int inputEntityCount;
} ECS;

void updateInputSystem(ECS *ecs, Entity *entity, InputComponent *input,
                       fixed_s32 deltaTime);
void updateBehaviorSystem(ECS *ecs, Entity *entity, AIComponent *ai);

void updatePhysicsSystem(Entity *entity, VelocityComponent *velocity,
                         AccelerationComponent *acceleration,
                         fixed_s32 deltaTime);
void updateCollisionSystem(Entity *entity, PositionComponent *position,
                           VelocityComponent *velocity, HitboxComponent *hitbox,
                           fixed_s32 deltaTime);

void updateMovementSystem(Entity *entity, PositionComponent *position,
                          VelocityComponent *velocity, fixed_s32 deltaTime);

void updateAnimationSystem(Entity *entity, AnimationComponent *animation);

void updateRenderSystem(ECS *ecs, Entity *entity, AnimationComponent *animation,
                        DrawingComponent *draw, TextComponent *text, const void *image);

#endif // !SYSTEMS_H
