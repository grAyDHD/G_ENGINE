#include "../data/dataSndData.h"
#include "audio/mixer.h"
#include "audio/modPlayer.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"

#include "core/interrupts.h"
#include "core/typedefs.h"
#include "ecs/components.h"
#include "ecs/systems.h"
#include "graphics/video.h"

#include "../build/Bedroom.h"
#include "../build/Sonic.h"
#include "core/timer.h"
#include "ecs/ecs.h"
#include "graphics/video.h"

extern void m3_Background(const void *src);

#define DSPS (*(volatile u16 *)0x04000004)

static ComponentStorage components;
static ECS ecs;
volatile fixed_s32 deltaTime;

int main() {
  DSPC = MODE3 | BG2;

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
    //    VBLANK();
    updateKeys();

    // MOD player update
    if (reload == 1) {
      modUpdate();
      reload = 0;
    }

    // ECS updates
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
    updateRenderSystem(&ecs, ecs.entity, BedroomBitmap);

    // Input handling
    if (keyTapped(B)) {
      playMod(MOD_BIT_Introtune);
    }

    deltaTime = 0;
  }
  return 0;
}
