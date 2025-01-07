#ifndef PHYS_H
#define PHYS_H

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

static inline int
checkForCollision(PositionComponent *posA, VelocityComponent *velA,
                  HitboxComponent *hitA, PositionComponent *posB,
                  VelocityComponent *velB, HitboxComponent *hitB) {
  int Ax1 = posA->x + velA->dx;
  int Ax2 = Ax1 + hitA->width;
  int Ay1 = posA->y + velA->dy;
  int Ay2 = Ay1 + hitA->height;
  int Bx1 = posB->x;
  int Bx2 = Bx1 + hitB->width;
  int By1 = posB->y;
  int By2 = By1 + hitB->height;
  return (Bx2 > Ax1 && Bx1 < Ax2 && By2 > Ay1 && By1 < Ay2);
}

void updateCollisionSystem(ECS *ecs, ComponentStorage *components) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    ecs->entity[id].flag &= ~COLLISION_DETECTED;
  }

  for (int idA = 0; idA < MAX_ENTITIES; idA++) {
    if (ecs->entity[idA].flag & ENABLE_COLLISIONS) {
      for (int idB = idA + 1; idB < MAX_ENTITIES; idB++) {
        if (ecs->entity[idB].flag & ENABLE_COLLISIONS) {
          if (checkForCollision(
                  &components->position[idA], &components->velocity[idA],
                  &components->hitbox[idA], &components->position[idB],
                  &components->velocity[idB], &components->hitbox[idB])) {
            ecs->entity[idA].flag |= COLLISION_DETECTED;
            ecs->entity[idB].flag |= COLLISION_DETECTED;
          }
        }
      }
    }
  }
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

#endif
