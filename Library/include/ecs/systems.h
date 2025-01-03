#ifndef SYSTEMS_H
#define SYSTEMS_H

typedef struct ECS ECS;
typedef struct ComponentStorage ComponentStorage;

// Physics system implementation
#define GRAVITY 1
#define MOVE_SPEED 2
#define SHOOT_VEL 5
#define JUMP_VELOCITY -10

void updateInputSystem(ECS *ecs, ComponentStorage *components);
void updateBehaviorSystem(ECS *ecs, ComponentStorage *components);
void updateRenderSystem(ECS *ecs, ComponentStorage *components);
void updateMovementSystem(ECS *ecs, ComponentStorage *components);

#endif // !SYSTEMS_H
