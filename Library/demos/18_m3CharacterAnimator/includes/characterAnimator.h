#ifndef CHARACTER_ANIMATOR_H
#define CHARACTER_ANIMATOR_H
#include "core/interrupts.h"
#include "core/typedefs.h"
#include "ecs/components.h"
#include "ecs/systems.h"
#include "graphics/video.h"

#include "../build/Bedroom.h"
#include "../build/Robo.h"
#include "core/timer.h"
#include "ecs/ecs.h"
#include "graphics/video.h"

extern void m3_Background(const void *src);

#endif // ECS_H
       //
//  extern void SpriteFrame32Bit(PositionComponent *positionData,
//                               AnimationComponent *animationData,
//                               const void *image);
//  extern void clearSpriteFrame(int x, int y, int size, const void *image);
