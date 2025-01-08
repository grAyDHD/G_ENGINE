#include "../includes/characterAnimator.h"
#include "ecs/components.h"
#include "ecs/entities.h"
#include "ecs/systems.h"
#include "graphics/draw.h"
#include "graphics/video.h"

static ComponentStorage components;
static ECS ecs;

typedef void (*fnptr)(void);
#define ISR *(fnptr *)(0x03007FFC) // put function address
// bit three vblank timer
#define DISPSTAT *(u16 *)0x4000004
// set bit 1 to enable interrupts
#define REG_IME *(u16 *)0x4000208
// first bit for vblank, |= 1
#define REG_IE *(u16 *)0x4000200
// set first bit to acknowledge interrupt |= 1
#define REG_IF *(u16 *)0x4000202

// Time handling
volatile s32 frame = 0;
void vblankISR() {
  if (frame == 60) {
    frame = 0;
  } else {
    frame++;
  }
  REG_IF = 1;
}

int main() {
  DSPC = MODE3 | BG2;
  ISR = vblankISR;   // tell the GBA where my isr is
  DISPSTAT = 1 << 3; // tell display to fire vblank interrupts
  REG_IE |= 1;       // tell GBA to accept vblank interrupts
  REG_IME |= 1;      // tell GBA to enable intterupts

  initEntitySystem(&ecs, &components);
  createPlayer(&ecs, RoboBitmap); // returns ID
  createNPC(&ecs, RoboBitmap);    // returns ID
  createScreenBorders(&ecs);
  m3_Background(BedroomBitmap);

  // ecs.entity[0].flag |= PHYSICS_FLAG;
  while (1) {
    VBLANK();

    clearSpriteFrames(&ecs, &components, BedroomBitmap);
    updateInputSystem(&ecs, ecs.entity, ecs.components->input);
    updateBehaviorSystem(&ecs, ecs.entity, ecs.components->ai);

    updatePhysicsSystem(ecs.entity, ecs.components->velocity,
                        ecs.components->acceleration);
    updateCollisionSystem(ecs.entity, ecs.components->position,
                          ecs.components->velocity, ecs.components->hitbox);
    updateMovementSystem(ecs.entity, ecs.components->position,
                         ecs.components->velocity);
    updateAnimationSystem(ecs.entity, ecs.components->animation);
    updateRenderSystem(&ecs, ecs.entity, ecs.components->animation,
                       ecs.components->draw);
  }

  return 0;
}
// current state, collisions working.  implement gravity
