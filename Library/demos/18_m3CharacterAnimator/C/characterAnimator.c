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

// Fixed point math utilities for GBA
typedef s32 fixed_t;  // 16.16 fixed point
typedef u32 ufixed_t; // Unsigned 16.16 for intermediate calculations

#define FIX_SHIFT 16
#define FIX_SCALE (1 << FIX_SHIFT)
#define FIX_MASK (FIX_SCALE - 1)
#define FIX_ONE FIX_SCALE

// Conversion macros
#define intToFixed(i) ((fixed_t)(i << FIX_SHIFT))
#define fixedToInt(f) ((s32)(f >> FIX_SHIFT))

// Multiplication using 32-bit operations only
static inline fixed_t fixedMul(fixed_t a, fixed_t b) {
  // Pre-shift one operand to prevent overflow
  // and maintain precision in middle bits
  fixed_t result;
  result = ((a >> 8) * (b >> 8));
  return result;
}

// Division using 32-bit operations
static inline fixed_t fixedDiv(fixed_t a, fixed_t b) {
  // Pre-shift numerator to maintain precision
  fixed_t result;
  result = ((a << 8) / (b >> 8));
  return result;
}

// Time handling
volatile s32 frame = 0;
volatile s32 frameDelta = 0;    // Frames since last update
volatile fixed_t deltaTime = 0; // Time in seconds as 16.16
//  deltaTime = fixedMul(intToFixed(frame), 1092);

void vblankISR() {
  if (frame == 60) {
    frame = 0;
  } else {
    frame++;
  }
  // Convert to seconds (approximately 1/60)
  // We use a pre-calculated constant to avoid division
  // 1/60 as fixed point ≈ 1092 in 16.16

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
