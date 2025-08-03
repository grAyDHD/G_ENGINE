#include "../includes/23_mPlayer.h"
#include "core/interrupts.h"
#include "core/typedefs.h"
extern void m3_Background(const void *src);

#define DSPS (*(volatile u16 *)0x04000004)

#define IN_VBLANK() (DISPSTAT & (1 << 0)) // Bit 0 = VBlank flag

static ComponentStorage components;
static ECS ecs;
volatile fixed_s32 deltaTime;

int main() {
  DSPC = MODE3 | BG2;
  ISR = irqDispatch;

  irqMaster(ON);
  // MOD player initialization
  initCharLookup();
  initializeFIFOInterrupts();
  modInit();

  // ECS initialization
  initializeVBI();

  m3_Background(BedroomBitmap);
  initEntitySystem(&ecs, &components);
  createPlayer(&ecs, SonicBitmap);
  ecs.entity[0].flag |= PHYSICS_FLAG;
  ecs.entity[0].flag |= DIRTY;
  createNPC(&ecs, SonicBitmap);
  createScreenBorders(&ecs);
  while (1) {
    updateKeys();

    // MOD player update
    if (reload == 1) {
      modUpdate();
      reload = 0;
    }

    //    VBLANK();
    // ECS updates

    if (deltaTime > 0) { // New frame available
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
      deltaTime = 0; // Reset after logic updates
    }

    // Rendering (only during VBlank to avoid visual glitches)
    if (IN_VBLANK()) {
      // u16 vcount_start = REG_VCOUNT;
      updateRenderSystem(&ecs, ecs.entity, BedroomBitmap);
    }

    // Input handling
    if (keyTapped(B)) {
      playMod(MOD_BIT_Introtune);
    }
  }
  return 0;
}
