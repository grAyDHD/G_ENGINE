// objectives:
// make sure jump isn't continuos on btn hold
// bounmce whn hitting walls
// three platforms- one bouncy, one giving jump boost, one cant jump from
//
#include "dmg.h"
#include "draw.h"
#include "gfx.h"
#include "in.h"
#include "phys.h"

// features to finish demo:
// three platforms to jump on,
// bitmap background
// game play loop: 'A' is jump, and should trigger "jump" sound effect.
// should only emit sound once while held, check if on ground when pressed, and
// jump slightly higher if held longer. when gravity is reversed, ball 'lands'
// on ceiling, and jumping works essentially the same but in reverse direction
// also, jump sound should have adjusted parameters  to reverse the sweep sound
// when gravity reversed. bumper buttons are used to launch the ball at a 45
// degree angle against gravity.  There should be momentum introduced to provide
// bouncing off of walls/platforms. A launch can be performed in the air, only
// if ground has been made contact with since the last use of the launch.
// gravity should have an accellerating effect, and the ball should bounce 0-3
// times depending on collision speed with the ground. can land one of three
// platforms, each emitting a unique collision sound.  launching into side of
// platform is effective to bounce the opposite direction left and right buttons
// move ball left and right at all times. holding 'B' reverses gravity until it
// is released.
void checkInput(struct Object *ball) {

  if (key_is_down(A)) {
    ball->vy = -SHOOT_VEL * gravityDirection;
  }

  if (key_is_down(B)) {
    gravityDirection = -1;
  } else {
    gravityDirection = 1;
  }

  if (key_is_down(SL)) {
    ball->vx = 0;
    ball->vy = 0;
    ball->ax = 0;
    ball->ay = 0;
  } else {

    applyGravity(ball);
    if (key_is_down(L)) {
      ball->vx = -MOVE_SPEED;
    } else if (key_is_down(R)) {
      ball->vx = MOVE_SPEED;
    } else {
      ball->vx = 0;
    }

    if (key_is_down(LT)) {
      ball->vx = -SHOOT_VEL;
      ball->vy = -SHOOT_VEL;
    } else if (key_is_down(RT)) {
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