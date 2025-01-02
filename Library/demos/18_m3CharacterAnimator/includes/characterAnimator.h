#ifndef CHARACTER_ANIMATOR_H
#define CHARACTER_ANIMATOR_H

#include "../build/Bedroom.h"
#include "../build/Robo.h"
#include "../build/RoboSmall.h"
#include "ECS.h"
extern void m3_Background(const void *src);
extern void SpriteFrame32Bit(PositionComponent *positionData,
                             AnimationComponent *animationData,
                             const void *image);
extern void clearSpriteFrame(int x, int y, int size, const void *image);

#endif // ECS_H
