#include "../includes/characterAnimator.h"
#include "core/interrupts.h"
#include "core/timer.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "graphics/draw.h"
#include "math/math.h"

static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime;
void initializeVBI() {
  ISR = vblankISR;   // tell the GBA where my isr is
  DISPSTAT = 1 << 3; // tell display to fire vblank interrupts
  REG_IE |= 1;       // tell GBA to accept vblank interrupts
  REG_IME |= 1;      // tell GBA to enable intterupts
}

int main() {
  DSPC = MODE3 | BG2;
  initializeVBI();

  initEntitySystem(&ecs, &components);
  int playerId = createPlayer(&ecs, RoboBitmap); // returns ID
  createScreenBorders(&ecs);

  m3_Background(BedroomBitmap);

  while (1) {
    VBLANK();

    clearSpriteFrames(&ecs, &components, BedroomBitmap);
    updateInputSystem(&ecs, ecs.entity, ecs.components->input, deltaTime);
    updatePhysicsSystem(ecs.entity, ecs.components->velocity,
                        ecs.components->acceleration, deltaTime);

    updateCollisionSystem(ecs.entity, ecs.components->position,
                          ecs.components->velocity, ecs.components->hitbox,
                          deltaTime);

    updateMovementSystem(ecs.entity, ecs.components->position,
                         ecs.components->velocity, deltaTime);
    updateAnimationSystem(ecs.entity, ecs.components->animation);
    updateRenderSystem(&ecs, ecs.entity, ecs.components->animation,
                       ecs.components->draw);
    deltaTime = 0;
  }

  return 0;
}

//  ecs.entity[0].flag |= PHYSICS_FLAG;

// createNPC(&ecs, RoboBitmap);    // returns ID
//
// 0000 0000 0000 0100
//    updateBehaviorSystem(&ecs, ecs.entity, ecs.components->ai);
