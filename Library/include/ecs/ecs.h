#ifndef ECS_H
#define ECS_H
#include "systems.h"

extern void clearSpriteFrame(int x, int y, int size, const void *image);
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
#endif // ECS_H
/*

// ecs.h
#ifndef ECS_H
#define ECS_H

#include "components.h"
#include "entities.h"
#include "systems.h"

#define MAX_ENTITIES 10
#define MAX_INPUT_ENTITIES 4

typedef struct ComponentStorage {
    PositionComponent position[MAX_ENTITIES];
    VelocityComponent velocity[MAX_ENTITIES];
    AccelerationComponent acceleration[MAX_ENTITIES];
    HitboxComponent hitbox[MAX_ENTITIES];
    AnimationComponent animation[MAX_ENTITIES];
    SpriteComponent sprite[MAX_ENTITIES];
    InputComponent input[MAX_INPUT_ENTITIES];
    AIComponent ai[MAX_ENTITIES];
    DrawingComponent draw[MAX_ENTITIES];
} ComponentStorage;

typedef struct ECS {
    ComponentStorage *components;
    Entity entity[MAX_ENTITIES];
    int inputEntityId[MAX_INPUT_ENTITIES];
    int nextEntityId;
    int entityCount;
    int inputEntityCount;
} ECS;

// Function declarations
void clearSpriteFrame(int x, int y, int size, const void *image);
void SpriteFrame32Bit(PositionComponent *positionData,
                     AnimationComponent *animationData,
                     const void *image);
int initEntitySystem(ECS *ecs, ComponentStorage *world);
void renderEntity(Entity* entity, int entityId);
void clearSpriteFrames(Entity* entity, ComponentStorage *world, const void
*image);

// Entity creation functions
int createEntity(ECS *ecs, int activeComponentMask);
int createPlayer(ECS *ecs, const void *spriteSheet);
int createNPC(ECS *ecs, const void *spriteSheet);
void createScreenBorders(ECS *ecs);

#endif // ECS_H
 */
