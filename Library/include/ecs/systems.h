#ifndef SYSTEMS_H
#define SYSTEMS_H

typedef struct ECS ECS;
typedef struct ComponentStorage ComponentStorage;

void updateInputSystem(ECS *ecs, ComponentStorage *components);
void updateBehaviorSystem(ECS *ecs, ComponentStorage *components);
void updatePhysicsSystem(ECS *ecs, ComponentStorage *components);
void updateRenderSystem(ECS *ecs, ComponentStorage *components);
void updateCollisionSystem(ECS *ecs, ComponentStorage *components);
void updateMovementSystem(ECS *ecs, ComponentStorage *components);
void updateAnimationSystem(ECS *ecs, ComponentStorage *components);

#endif // !SYSTEMS_H
