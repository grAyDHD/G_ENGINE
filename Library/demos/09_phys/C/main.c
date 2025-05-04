#include "../includes/include.h"
#include "core/interrupts.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "ecs/systems.h"
#include "graphics/video.h"
#include "math/math.h"
#include "physics/phys.h"
#include "core/timer.h"

// implement single jump with on ground flag.
// on input handler, only accept jump input if on ground flag is active
// in input handler jump case, set velocity AND clear on ground flag
// upon collision with GROUND ONLY, set on ground flag

// current commit goal:
// determine cause of gravity pushing through floor and resetting position repeatedly, 
// this will determine action for next commit.  idea: alternate planning and execution commits?



static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime = 0;

int main() {
  DSPC = MODE3 | BG2;

  initializeVBI();
  initEntitySystem(&ecs, &components);

  int ball = initBall(&ecs, &components);
  createScreenBorders(&ecs);

  Coordinate previousCorner = {
      .x = FIXED_TO_INT(components.position[ball].x),
      .y = FIXED_TO_INT(components.position[ball].y)}; // Track the previous
  Coordinate corner = {
      .x = FIXED_TO_INT(components.position[ball].x),
      .y = FIXED_TO_INT(components.position[ball].y)}; // Track the previous

  previousCorner.x = corner.x;
  previousCorner.y = corner.y;

  while (1) {
    // vblank interrupts calculate delta time in background, for accurate calculations should the processer/cpu be stressed
    // VBLANK waits for the next drawing period 
    VBLANK();

    updateInputSystem(&ecs, ecs.entity, components.input, deltaTime);
    // applies gravity every frame
    updatePhysicsSystem(ecs.entity, components.velocity,
                        components.acceleration, deltaTime);
    updateMovementSystem(ecs.entity, components.position, components.velocity,
                         deltaTime);
    // when player is pushed by gravity when on floor, it is pushed past the floor
    // is this issue with gravity in physics system, timing in movement system, or with resolutions handled
    // by collision system
    updateCollisionSystem(ecs.entity, components.position, components.velocity,
                          components.hitbox, deltaTime);
    // reflects appropriate updates to animation system
    updateAnimationSystem(ecs.entity, components.animation);
    // reflects appropriate updates to rendering system- used for various drawing functions
    updateRenderSystem(&ecs, ecs.entity, components.animation, components.draw);
    corner.x = FIXED_TO_INT(components.position[ball].x);
    corner.y = FIXED_TO_INT(components.position[ball].y);

    previousCorner = corner;
    deltaTime = 0;

  }
  return 0;
}

// Commit plan:
// ensure gravity always applies, and collision system sets on ground flag and resolves movement
