#include "../includes/characterAnimator.h"
#include "core/interrupts.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "graphics/draw.h"

static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime;
void initializeVBI();

//  createNPC(&ecs, RoboBitmap);
int main() {
  DSPC = MODE3 | BG2;
  initializeVBI();
  m3_Background(BedroomBitmap);
  initEntitySystem(&ecs, &components);
  createPlayer(&ecs, RoboBitmap);
  createScreenBorders(&ecs);

  ecs.entity[0].flag |= PHYSICS_FLAG;
  while (1) {
    VBLANK();
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
    updateRenderSystem(&ecs, ecs.entity, ecs.components->animation,
                       ecs.components->draw);
    deltaTime = 0;
  }

  return 0;
}

void initializeVBI() {

  ISR = vblankISR;   // tell the GBA where my isr is
  DISPSTAT = 1 << 3; // tell display to fire vblank interrupts
  REG_IE |= 1;       // tell GBA to accept vblank interrupts
  REG_IME |= 1;      // tell GBA to enable intterupts
}
