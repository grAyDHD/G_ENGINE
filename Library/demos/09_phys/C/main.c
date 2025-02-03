#include "../includes/09_phys.h"
#include "core/interrupts.h"
#include "core/timer.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "ecs/entities.h"
#include "ecs/systems.h"
#include "graphics/draw.h"
#include "graphics/video.h"
#include "input/in.h"
#include "math/math.h"
#include "physics/phys.h"

// goal for next commit:  implement single jump with on ground flag.
// on input handler, only accept jump input if on ground flag is active
// in input handler jump case, set velocity AND clear on ground flag
// upon collision with GROUND ONLY, set on ground flag

static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime = 0;

// ball animation states
// jump, land, squishLeft squishRight
//
int main() {
  DSPC = MODE3 | BG2;
  initializeVBI();
  initEntitySystem(&ecs, &components);

  int ball = initBall(&ecs, &components);
  createScreenBorders(&ecs);
  // struct Object ball = {120, 80, 0, 0, 0, 0};

  Coordinate previousCorner = {
      .x = FIXED_TO_INT(components.position[ball].x),
      .y = FIXED_TO_INT(components.position[ball].y)}; // Track the previous
  Coordinate corner = {
      .x = FIXED_TO_INT(components.position[ball].x),
      .y = FIXED_TO_INT(components.position[ball].y)}; // Track the previous

  while (1) {
    VBLANK();

    // clear previous position
    drawRect(previousCorner, BALL_SIZE, BALL_SIZE, 0x0000);

    updateInputSystem(&ecs, ecs.entity, components.input, deltaTime);
    updatePhysicsSystem(ecs.entity, components.velocity,
                        components.acceleration, deltaTime);
    updateMovementSystem(ecs.entity, components.position, components.velocity,
                         deltaTime);
    updateCollisionSystem(ecs.entity, components.position, components.velocity,
                          components.hitbox, deltaTime);
    corner.x = FIXED_TO_INT(components.position[ball].x);
    corner.y = FIXED_TO_INT(components.position[ball].y);

    drawRect(corner, BALL_SIZE, BALL_SIZE, 0x03E0);
    previousCorner = corner;
    deltaTime = 0;

    //    simpleWait(25);
  }
  return 0;
}
