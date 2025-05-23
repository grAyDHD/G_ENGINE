#include "../includes/ui.h"
#include "ecs/entities.h"

static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime;


int main() {
  DSPC = MODE3 | BG2;
  initializeVBI();
 // m3_Background(BedroomBitmap);
  initEntitySystem(&ecs, &components);
  createPlayer(&ecs, SonicBitmap);
  ecs.entity[0].flag |= PHYSICS_FLAG | ON_GROUND;
  //createNPC(&ecs, RoboBitmap);
  createScreenBorders(&ecs);

  while (1) {
    VBLANK();
    clearSpriteFrames(&ecs, &components, 0);
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
