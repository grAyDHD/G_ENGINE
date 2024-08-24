#ifndef PHYS_H
#define PHYS_H

#define GRAVITY 1    // Gravity constant
#define SW 240       // Screen width
#define SH 160       // Screen height
#define BALL_SIZE 4  // Size of the ball
#define MOVE_SPEED 2 // Speed of movement when pressing left/right
#define SHOOT_VEL 5  // Initial velocity when shooting with bumpers
#define JUMP_VELOCITY -10

struct Object {
  int x;
  int y;
  int vx;
  int vy;
  int ax;
  int ay;
  int onGround;
};

void applyGravity(struct Object *obj);
void updateMovement(struct Object *obj);
void handleCollisions(struct Object *obj);

#endif
