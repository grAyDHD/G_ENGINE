#include "ecs/components.h"
#include "ecs/ecs.h"
#include "ecs/entities.h"
#include "graphics/draw.h"
#include "graphics/video.h"
#include "input/in.h"

#define GRAVITY 1    // Gravity constant
#define MOVE_SPEED 2 // Speed of movement when pressing left/right
#define SHOOT_VEL 5  // Initial velocity when shooting with bumpers
#define JUMP_VELOCITY -10
int BALL_SIZE = 4;

extern int gravityDirection;

// void applyGravity(struct Object *obj);
// void updateMovement(struct Object *obj);
// void handleCollisions(struct Object *obj, int BALL_SIZE);

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

void handleCollisions(ECS *obj, int BALL_SIZE) {
  if (gravityDirection == 1) {
    if (obj->components->position[0].y >= SH - BALL_SIZE) {
      obj->components->position[0].y = SH - BALL_SIZE;
      obj->components->velocity[0].dy = -(obj->components->velocity[0].dy / 2);
      obj->entity[0].flag |= ON_GROUND;
    } else if (obj->components->position[0].y <= 0) {
      obj->components->velocity[0].dy = 0;
      obj->entity[0].flag &= ~ON_GROUND; // Clear ON_GROUND flag
    } else {
      obj->entity[0].flag &= ~ON_GROUND; // Clear ON_GROUND flag
    }
  } else if (gravityDirection == -1) {
    if (obj->components->position[0].y <= 0) {
      obj->components->position[0].y = 0;
      obj->components->velocity[0].dy = -(obj->components->velocity[0].dy / 2);
      obj->entity[0].flag |= ON_GROUND;
    } else if (obj->components->position[0].y >= SH - BALL_SIZE) {
      obj->components->position[0].y = SH - BALL_SIZE;
      obj->components->velocity[0].dy = 0;

      obj->entity[0].flag &= ~ON_GROUND; // Clear ON_GROUND flag
    } else {
      obj->entity[0].flag &= ~ON_GROUND; // Clear ON_GROUND flag
    }
  }

  if (obj->components->position[0].x < 0) {
    obj->components->position[0].x = 0;
    obj->components->velocity[0].dx = 0;
  }
  if (obj->components->position[0].x >= SW - BALL_SIZE) {
    obj->components->position[0].x = SW - BALL_SIZE;
    obj->components->velocity[0].dx = 0;
  }
}
/*
 */

void checkInput(ECS *ball) {

  if (keyDown(A)) {
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
  } else {

    applyGravity(ball);
    if (keyDown(L)) {
      ball->components->velocity[0].dx = -MOVE_SPEED;
    } else if (keyDown(R)) {
      ball->components->velocity[0].dx = MOVE_SPEED;
    } else {
      ball->components->velocity[0].dx = 0;
    }

    if (keyDown(LT)) {
      ball->components->velocity[0].dx = -SHOOT_VEL;
      ball->components->velocity[0].dy = -SHOOT_VEL;
    } else if (keyDown(RT)) {
      ball->components->velocity[0].dx = SHOOT_VEL;
      ball->components->velocity[0].dy = -SHOOT_VEL;
    }
  }
}

static ComponentStorage world;
static ECS entitySystem;

int main() {
  DSPC = MODE3 | BG2;
  initEntitySystem(&entitySystem, &world);
  int ball =
      createEntity(&entitySystem, POSITION_COMPONENT | VELOCITY_COMPONENT |
                                      ACCELERATION_COMPONENT | INPUT_COMPONENT |
                                      ENABLE_INPUT | ENABLE_PHYSICS |
                                      ENABLE_COLLISIONS | ON_GROUND);

  entitySystem.components->position[ball].x = 120;
  entitySystem.components->position[ball].y = 80;

  entitySystem.components->velocity[ball].dx = 120;
  entitySystem.components->velocity[ball].dy = 80;

  entitySystem.components->acceleration[ball].ax = 120;
  entitySystem.components->acceleration[ball].ay = 80;

  Coordinate previousCorner = {entitySystem.components->position[ball].x,
                               entitySystem.components->position[ball].y};
  Coordinate corner = {entitySystem.components->position[ball].x,
                       entitySystem.components->position[ball].y};

  while (1) {

    updateKeys();
    VBLANK();

    // erase current position
    drawRect(previousCorner, BALL_SIZE, BALL_SIZE, 0x0000);

    checkInput(&entitySystem);

    // update position based on input/physics
    updateMovement(&entitySystem);
    handleCollisions(&entitySystem, BALL_SIZE);
    corner.x = entitySystem.components->position[ball].x;
    corner.y = entitySystem.components->position[ball].y;

    // draw in new position
    drawRect(corner, BALL_SIZE, BALL_SIZE, 0x03E0);
    previousCorner = corner;
    //
  }
  return 0;
}
