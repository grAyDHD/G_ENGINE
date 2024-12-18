#include "engine.h"

void checkInput(struct Object *ball) {

  if (key_held(A)) {
    ball->vy = -SHOOT_VEL * gravityDirection;
  }

  if (key_held(B)) {
    gravityDirection = -1;
  } else {
    gravityDirection = 1;
  }

  if (key_held(SL)) {
    ball->vx = 0;
    ball->vy = 0;
    ball->ax = 0;
    ball->ay = 0;
  } else {

    applyGravity(ball);
    if (key_held(L)) {
      ball->vx = -MOVE_SPEED;
    } else if (key_held(R)) {
      ball->vx = MOVE_SPEED;
    } else {
      ball->vx = 0;
    }

    if (key_held(LT)) {
      ball->vx = -SHOOT_VEL;
      ball->vy = -SHOOT_VEL;
    } else if (key_held(RT)) {
      ball->vx = SHOOT_VEL;
      ball->vy = -SHOOT_VEL;
    }
  }
}

int BALL_SIZE = 20;

int main() {
  DSPC = MODE3 | BG2;
  initSoundSystem();
  defaultCH1Sound();
  TIMED();

  struct Object ball = {120, 80, 0, 0, 0, 0};
  Coordinate prevCrnr = {ball.x, ball.y}; // Track the previous position
  Coordinate crnr = {ball.x, ball.y};

  while (1) {
    key_poll();
    // erase current position
    drawRect(prevCrnr, BALL_SIZE, BALL_SIZE, 0x0000);
    checkInput(&ball);
    // update position based on input/physics
    updateMovement(&ball);
    handleCollisions(&ball, BALL_SIZE);
    crnr.x = ball.x;
    crnr.y = ball.y;
    // draw in new position
    drawRect(crnr, BALL_SIZE, BALL_SIZE, 0x03E0);
    prevCrnr = crnr;
    VBLANK();
  }
  return 0;
}
