#include "../includes/ui.h"
#include "../build/Bedroom.h"
#include "../build/Robo.h"
#include "input/in.h"
#include "ecs/entities.h"
#include "graphics/draw.h"
#include "input/in.h"

static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime;

int main() {
  DSPC = MODE3 | BG2;
  initializeVBI();
  fillScreen(0x0000);
  initEntitySystem(&ecs, &components);
  createPlayer(&ecs, SonicBitmap);
//  ecs.entity[0].flag |= PHYSICS_FLAG; //ON_GROUND;
  createScreenBorders(&ecs);

  while (1) {
    VBLANK();
    fillScreen(0x0000);


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
