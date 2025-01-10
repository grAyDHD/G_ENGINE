#include "ecs/components.h"
#include "graphics/draw.h"
#include "graphics/video.h"
#include "input/in.h"
#include "physics/phys.h"

void checkInput(struct Object *ball) {

  if (keyDown(A)) {
    ball->vy = -SHOOT_VEL * gravityDirection;
  }

  if (keyDown(B)) {
    gravityDirection = -1;
  } else {
    gravityDirection = 1;
  }

  if (keyDown(SL)) {
    ball->vx = 0;
    ball->vy = 0;
    ball->ax = 0;
    ball->ay = 0;
  } else {

    applyGravity(ball);
    if (keyDown(L)) {
      ball->vx = -MOVE_SPEED;
    } else if (keyDown(R)) {
      ball->vx = MOVE_SPEED;
    } else {
      ball->vx = 0;
    }

    if (keyDown(LT)) {
      ball->vx = -SHOOT_VEL;
      ball->vy = -SHOOT_VEL;
    } else if (keyDown(RT)) {
      ball->vx = SHOOT_VEL;
      ball->vy = -SHOOT_VEL;
    }
  }
}

int BALL_SIZE = 20;

int main() {
  DSPC = MODE3 | BG2;

  struct Object ball = {120, 80, 0, 0, 0, 0};
  PositionComponent previousCorner = {ball.x,
                                      ball.y}; // Track the previous position
  PositionComponent corner = {ball.x, ball.y};

  while (1) {
    updateKeys();
    VBLANK();
    float a = 4.5;
    drawRect((PositionComponent){.x = a, .y = a}, a, a, a * a);
    // erase current position
    drawRect(previousCorner, BALL_SIZE, BALL_SIZE, 0x0000);
    checkInput(&ball);
    // update position based on input/physics
    updateMovement(&ball);
    handleCollisions(&ball, BALL_SIZE);
    corner.x = ball.x;
    corner.y = ball.y;
    // draw in new position
    drawRect(corner, BALL_SIZE, BALL_SIZE, 0x03E0);
    previousCorner = corner;
  }
  return 0;
}
