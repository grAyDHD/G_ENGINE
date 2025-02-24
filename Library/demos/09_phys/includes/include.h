#ifndef PHYS_H
#define PHYS_H

#include "ecs/components.h"
#include "math/math.h"

// #define GRAVITY 1    // Gravity constant

#define MOVE_SPEED INT_TO_FIXED(90)
#define SHOOT_VEL INT_TO_FIXED(120)
#define JUMP_VEL INT_TO_FIXED(120)

extern int gravityDirection;
extern int ballSize;

void playerInput(ECS *ecs, int entityId);
void drawBall(ECS *ecs, int entityId);
int initBall(ECS *ecs, ComponentStorage *components);

#endif
