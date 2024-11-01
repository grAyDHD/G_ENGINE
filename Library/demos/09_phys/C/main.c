// objectives:
// make sure jump isn't continuos on btn hold
// bounmce whn hitting walls
// three platforms- one bouncy, one giving jump boost, one cant jump from
//
#include "dmg.h"
#include "draw.h"
#include "gfx.h"
#include "input.h"
#include "phys.h"

int gravityDirection = 1;
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

  if ((KEYS & A) == 0) {
    ball->vy = -SHOOT_VEL * gravityDirection;
  }

  if ((KEYS & B) == 0) {
    gravityDirection = -1;
  } else {
    gravityDirection = 1;
  }

  if ((KEYS & SL) == 0) {
    ball->vx = 0;
    ball->vy = 0;
    ball->ax = 0;
    ball->ay = 0;
  } else {

    applyGravity(ball);
    if ((KEYS & L) == 0) {
      ball->vx = -MOVE_SPEED;
    } else if ((KEYS & R) == 0) {
      ball->vx = MOVE_SPEED;
    } else {
      ball->vx = 0;
    }

    if ((KEYS & LS) == 0) {
      ball->vx = -SHOOT_VEL;
      ball->vy = -SHOOT_VEL;
    } else if ((KEYS & RS) == 0) {
      ball->vx = SHOOT_VEL;
      ball->vy = -SHOOT_VEL;
    }
  }
}

int main() {
  DSPC = MODE3 | BG2;
  initSoundSystem();
  defaultCH1Sound();
  TIMED();

  struct Object ball = {120, 80, 0, 0, 0, 0};
  Coordinate prevCrnr = {ball.x, ball.y}; // Track the previous position

  while (1) {
    drawRect(prevCrnr, BALL_SIZE, BALL_SIZE, 0x0000, VRAM);
    checkInput(&ball);
    updateMovement(&ball);
    handleCollisions(&ball);
    Coordinate crnr = {ball.x, ball.y};
    drawRect(crnr, BALL_SIZE, BALL_SIZE, 0x03E0, VRAM);
    prevCrnr = crnr;
    waitVBLANK();
  }
  return 0;
}
