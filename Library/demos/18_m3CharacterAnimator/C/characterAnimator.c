#include "../includes/characterAnimator.h"
#include "core/interrupts.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "graphics/draw.h"

static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime;

int main() {
  DSPC = MODE3 | BG2;
  initializeVBI();
  m3_Background(BedroomBitmap);
  initEntitySystem(&ecs, &components);
  createPlayer(&ecs, RoboBitmap);
  ecs.entity[0].flag |= PHYSICS_FLAG;
  createNPC(&ecs, RoboBitmap);
  createScreenBorders(&ecs);

  while (1) {
    VBLANK();

    //    if (DSPS & 0x0001)
    clearSpriteFrames(&ecs, &components, BedroomBitmap);
    updateInputSystem(&ecs, ecs.entity, ecs.components->input, deltaTime);
    updateBehaviorSystem(&ecs, ecs.entity, ecs.components->ai);
    updatePhysicsSystem(ecs.entity, ecs.components->velocity,
                        ecs.components->acceleration, deltaTime);
    updateMovementSystem(ecs.entity, ecs.components->position,
                         ecs.components->velocity, deltaTime);
    updateCollisionSystem(ecs.entity, ecs.components->position,
                          ecs.components->velocity, ecs.components->hitbox,
                          deltaTime);
    updateAnimationSystem(ecs.entity, ecs.components->animation);
    updateRenderSystem(&ecs, ecs.entity);
    deltaTime = 0;
  }

  return 0;
}
