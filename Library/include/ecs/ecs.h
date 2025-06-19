#ifndef ECS_H
#define ECS_H
#include "ecs/components.h"
#include "systems.h"

// todo: clearSpriteFrame vs clearSpriteFrames? remove SpriteFrame32Bit? ensure clearSpriteFrame handles edge cases
//clears variable width sprite
extern void clearSpriteFrame(fixed_s32 x, fixed_s32 y, int size,
                             const void *image);
// draws 32x32 sprite
extern void SpriteFrame32Bit(PositionComponent *positionData,
                             AnimationComponent *animationData,
                             const void *image);

int initEntitySystem(ECS *ecs, ComponentStorage *world);

void renderEntity(ECS *ecs, int entityId);
void renderEntityText(ECS *ecs, int entityId);

void clearSpriteFrames(ECS *system, ComponentStorage *world, const void *image);

int createEntity(ECS *ecs, int activeComponentMask);
int createPlayer(ECS *ecs, const void *spriteSheet);
int createNPC(ECS *ecs, const void *spriteSheet);
void createScreenBorders(ECS *ecs);

void playerInputHandler(ECS *ecs, int entityId);
void patrolBehavior(ECS *ecs, int entityId);

#endif // ECS_H
