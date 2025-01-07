#include "core/typedefs.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "ecs/entities.h"
#include "ecs/systems.h"
#include "graphics/draw.h"
#include "graphics/video.h"
#include "input/in.h"

#define GRAVITY 1    // Gravity constant
#define MOVE_SPEED 2 // Speed of movement when pressing left/right
#define SHOOT_VEL 10 // Initial velocity when shooting with bumpers
#define JUMP_VELOCITY -10
int BALL_SIZE = 4;

extern int gravityDirection;

int gravityDirection = 1;

void applyGravity(ECS *obj) {
  obj->components->acceleration[0].ay = GRAVITY * gravityDirection;
}

void updateMovement(ECS *obj) {
  obj->components->velocity[0].dx += obj->components->acceleration[0].ax;
  obj->components->velocity[0].dy += obj->components->acceleration[0].ay;
  obj->components->position[0].x += obj->components->velocity[0].dx;
  obj->components->position[0].y += obj->components->velocity[0].dy;
}

void playerInput(ECS *ball, int entityId) {

  if (keyTapped(A)) {
    ball->components->velocity[0].dy = -SHOOT_VEL * gravityDirection;
  }

  if (keyDown(B)) {
    gravityDirection = -1;
  } else {
    gravityDirection = 1;
  }

  if (keyDown(SL)) {
    ball->components->velocity[0].dx = 0;
    ball->components->velocity[0].dy = 0;
    ball->components->acceleration[0].ax = 0;
    ball->components->acceleration[0].ay = 0;
  }

  if (keyDown(L)) {
    ball->components->velocity[0].dx = -MOVE_SPEED;
  } else if (keyDown(R)) {
    ball->components->velocity[0].dx = MOVE_SPEED;
  } else {
    ball->components->velocity[0].dx = 0;
  }

  if (keyDown(LT)) {
    ball->components->velocity[0].dx = -SHOOT_VEL;
  } else if (keyDown(RT)) {
    ball->components->velocity[0].dx = SHOOT_VEL;
  }
  if (keyTapped(LT)) {
    ball->components->velocity[0].dy = -SHOOT_VEL;
  } else if (keyTapped(RT)) {
    ball->components->velocity[0].dy = -SHOOT_VEL;
  }
}

static ComponentStorage world;
static ECS entitySystem;

void drawSquare(ECS *ecs, int entityId) {
  drawRect(ecs->components->position[entityId], BALL_SIZE, BALL_SIZE, 0x03E0);
}

int main() {
  DSPC = MODE3 | BG2;
  initEntitySystem(&entitySystem, &world);
  int ball = createEntity(
      &entitySystem, POSITION_COMPONENT | VELOCITY_COMPONENT |
                         ACCELERATION_COMPONENT | INPUT_COMPONENT |
                         DRAWING_COMPONENT | ENABLE_INPUT | ENABLE_PHYSICS |
                         TRIGGERS_COLLISIONS | DETECTS_COLLISIONS | ON_GROUND);
  entitySystem.components->position[ball] = (PositionComponent){120, 80};
  entitySystem.components->velocity[ball] = (VelocityComponent){0, 0};
  entitySystem.components->acceleration[ball] = (AccelerationComponent){0, 0};
  entitySystem.components->input[0].handleInput = playerInput;
  entitySystem.components->draw[0].drawingRoutine = drawSquare;

  // create floor entity id = 1
  createEntity(&entitySystem, BOUNDARY_ENTITY);
  //(POSITION_COMPONENT | HITBOX_COMPONENT | TRIGGERS_COLLISIONS)
  // need not check collisions itself, but causes collisions/is checked for
  // collisions by other collision DETECTING entities
  entitySystem.components->position[1] = (PositionComponent){0, 160};
  entitySystem.components->hitbox[1] = (HitboxComponent){BALL_SIZE, BALL_SIZE};
  entitySystem.components->acceleration[1] = (AccelerationComponent){240, 1};

  PositionComponent previousCorner = {
      entitySystem.components->position[ball].x,
      entitySystem.components->position[ball].y};
  PositionComponent corner = {entitySystem.components->position[ball].x,
                              entitySystem.components->position[ball].y};

  PositionComponent test = {.x = 20, .y = 100};
  while (1) {
    VBLANK();

    // erase current position.  update to clear all drawn sprites,
    // clearRenderSystem?
    drawRect(previousCorner, BALL_SIZE, BALL_SIZE, 0x0000);

    updateInputSystem(&entitySystem, entitySystem.components);
    applyGravity(&entitySystem);
    //    updateCollisionSystem(&entitySystem, entitySystem.components);
    //    updateMovementSystem(&entitySystem, entitySystem.components);
    //    instead of sprites updateRenderSystem
    updateMovement(&entitySystem);
    //     handleCollisions(&entitySystem, BALL_SIZE);
    corner.x = entitySystem.components->position[ball].x;
    corner.y = entitySystem.components->position[ball].y;

    updateRenderSystem(&entitySystem, entitySystem.components);

    previousCorner = corner;
  }
  return 0;
}
