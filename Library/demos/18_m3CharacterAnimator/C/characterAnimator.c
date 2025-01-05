#include "../includes/characterAnimator.h"
#include "ecs/systems.h"
#include "graphics/draw.h"
#include "graphics/video.h"

static ComponentStorage world;
static ECS entitySystem;

typedef void (*fnptr)(void);
#define ISR *(fnptr *)(0x03007FFC)

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

  // acknowledge intterupt by setting bit 1 to 1  // first bit for vblank, |= 1
  REG_IF = 1;
}

int main() {
  DSPC = MODE3 | BG2;

  ISR = vblankISR;   // tell the GBA where my isr is
  DISPSTAT = 1 << 3; // tell display to fire vblank interrupts
  REG_IE |= 1;       // tell GBA to accept vblank interrupts
  REG_IME |= 1;      // tell GBA to enable intterupts

  initEntitySystem(&entitySystem, &world);
  createPlayer(&entitySystem, &RoboBitmap); // returns ID
  createNPC(&entitySystem, &RoboBitmap);    // returns ID
  m3_Background(BedroomBitmap);

  while (1) {
    VBLANK();

    clearSpriteFrames(&entitySystem, &world, BedroomBitmap);
    updateInputSystem(&entitySystem, &world);
    updateBehaviorSystem(&entitySystem, &world);
    // implement fixed point system for delta time, then gravity
    //     updateGravitySystem(&entitySystem, &world); apply force downwards
    //     based on delta time
    updateCollisionSystem(&entitySystem, &world);
    updateMovementSystem(&entitySystem, &world);
    updateAnimationSystem(&entitySystem, &world);
    updateRenderSystem(&entitySystem, &world);
  }

  return 0;
}
// current state, collisions working.  implement gravity
