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

static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime = 0;

int BALL_SIZE = 20;

// void (*drawingRoutine)(ECS *ecs, int entityId);

void drawBall(ECS *ecs, int entityId) {
  // migrate drawing of ball into this function
}

int initBall(ECS *ecs, ComponentStorage *components) {
  int ball = createEntity(
      ecs, POSITION_COMPONENT | VELOCITY_COMPONENT | ACCELERATION_COMPONENT |
               INPUT_COMPONENT | HITBOX_COMPONENT | DRAWING_COMPONENT |
               ENABLE_INPUT | ENABLE_PHYSICS | PHYSICS_FLAG |
               DETECTS_COLLISIONS | TRIGGERS_COLLISIONS);

  components->position[ball] =
      (PositionComponent){.x = INT_TO_FIXED(120), .y = INT_TO_FIXED(80)};
  components->velocity[ball] = (VelocityComponent){.dx = 0, .dy = 0};
  components->acceleration[ball] = (AccelerationComponent){.ax = 0, .ay = 0};

  ecs->components->input[ball].handleInput = playerInputHandler;
  ecs->components->draw[ball].drawingRoutine = drawBall;

  // add input handling
  // add drawing

  return ball;
}

int main() {
  DSPC = MODE3 | BG2;
  initializeVBI();
  initEntitySystem(&ecs, &components);

  int ball = initBall(&ecs, &components);
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
    //   handleCollisions(&ball, BALL_SIZE);
    corner.x = FIXED_TO_INT(components.position[ball].x);
    corner.y = FIXED_TO_INT(components.position[ball].y);

    drawRect(corner, BALL_SIZE, BALL_SIZE, 0x03E0);
    previousCorner = corner;
    deltaTime = 0;

    //    simpleWait(25);
  }
  return 0;
}
