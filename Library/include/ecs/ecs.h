#ifndef ECS_H
#define ECS_H
#include "ecs/components.h"
#include "systems.h"

extern void clearSpriteFrame(fixed_s32 x, fixed_s32 y, int size,
                             const void *image);
extern void SpriteFrame32Bit(PositionComponent *positionData,
                             AnimationComponent *animationData,
                             const void *image);

int initEntitySystem(ECS *ecs, ComponentStorage *world);
void renderEntity(ECS *ecs, int entityId);
void clearSpriteFrames(ECS *system, ComponentStorage *world, const void *image);

int createEntity(ECS *ecs, int activeComponentMask);
int createPlayer(ECS *ecs, const void *spriteSheet);
int createNPC(ECS *ecs, const void *spriteSheet);
void createScreenBorders(ECS *ecs);

void playerInputHandler(ECS *ecs, int entityId, fixed_s32 deltaTime);
void patrolBehavior(ECS *ecs, int entityId);

// void patrolBehavior(AnimationComponent *animation, VelocityComponent
// *velocity,
//                   AIComponent *ai);
//

#endif // ECS_H
