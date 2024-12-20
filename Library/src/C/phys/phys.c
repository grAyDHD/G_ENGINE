#include "physics/phys.h"
#include "gfx/gfx.h"

int gravityDirection = 1;
void applyGravity(struct Object *obj) { obj->ay = GRAVITY * gravityDirection; }

void updateMovement(struct Object *obj) {
  obj->vx += obj->ax;
  obj->vy += obj->ay;
  obj->x += obj->vx;
  obj->y += obj->vy;
}

void handleCollisions(struct Object *obj, int BALL_SIZE) {
  if (gravityDirection == 1) {
    if (obj->y >= SH - BALL_SIZE) {
      obj->y = SH - BALL_SIZE;
      obj->vy = -(obj->vy / 2);
      obj->onGround = 1;
    } else if (obj->y <= 0) {
      obj->vy = 0;
      obj->onGround = 0;
    } else {
      obj->onGround = 0;
    }
  }

  else if (gravityDirection == -1) {
    if (obj->y <= 0) {
      obj->y = 0;
      obj->vy = -(obj->vy / 2);
      obj->onGround = 1;
    } else if (obj->y >= SH - BALL_SIZE) {
      obj->y = SH - BALL_SIZE;
      obj->vy = 0;
      obj->onGround = 0;
    } else {
      obj->onGround = 0;
    }
  }

  if (obj->x < 0) {
    obj->x = 0;
    obj->vx = 0;
  }
  if (obj->x >= SW - BALL_SIZE) {
    obj->x = SW - BALL_SIZE;
    obj->vx = 0;
  }
}
