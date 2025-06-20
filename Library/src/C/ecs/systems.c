#include "ecs/systems.h"
#include "../../include/ecs/ecs.h"
#include "../../include/input/in.h"
#include "core/typedefs.h"
#include "ecs/components.h"

#include "graphics/draw.h"
#include "ecs/entities.h"
#include "math/math.h"
#include "graphics/m3Text.h"

void updateInputSystem(ECS *ecs, Entity *entity, InputComponent *input,
                       fixed_s32 deltaTime) {
  updateKeys();
  if (entity[0].flag & ACTIVE) {
    input[0].handleInput(ecs, 0);
  }

  if (entity[1].flag & ACTIVE) {
    input[1].handleInput(ecs, 1);
  }
}

void updateBehaviorSystem(ECS *ecs, Entity *entity, AIComponent *ai) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    if (entity[i].flag & (AI_COMPONENT & ACTIVE)) {
      ai[i].aiBehavior(ecs, i);
    }
  }
};

#define FRICTION_COEFFICIENT 0x0000B000 
// (INT_TO_FIXED(63) >> 7)

void updatePhysicsSystem(Entity *entity, VelocityComponent *velocity,
                         AccelerationComponent *acceleration,
                         fixed_s32 deltaTime) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (!(entity[id].flag & ACTIVE))
      continue;

    if (!(entity[id].flag & PHYSICS_FLAG))
      continue;

    if ((entity[id].flag & ENABLE_GRAVITY) && !(entity[id].flag & ON_GROUND)) {
      acceleration[id].ay += GRAVITY;
    }

    // below, if platformer
    /*
    if (entity[id].flag & ON_GROUND) {
      velocity[id].dx = MULT(velocity[id].dx, FRICTION_COEFFICIENT);
    }
    */

    // if not platformer:
    velocity[id].dx = MULT(velocity[id].dx, FRICTION_COEFFICIENT);
    velocity[id].dy = MULT(velocity[id].dy, FRICTION_COEFFICIENT);

    velocity[id].dx += MULT(acceleration[id].ax, deltaTime);
    velocity[id].dy += MULT(acceleration[id].ay, deltaTime);

    acceleration[id].ax = 0;
    acceleration[id].ay = 0;
  }
}

void updateMovementSystem(Entity *entity, PositionComponent *position,
                          VelocityComponent *velocity, fixed_s32 deltaTime) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (!(entity[id].flag & ACTIVE)) continue;
    if (entity[id].flag & VELOCITY_COMPONENT) {
      if (velocity[id].dx != 0 || velocity[id].dy != 0) {
        position[id].prevX = position[id].x;
        position[id].prevY = position[id].y;
        
        position[id].x += (MULT(velocity[id].dx, deltaTime));
        position[id].y += (MULT(velocity[id].dy, deltaTime));
        entity[id].flag |= DIRTY;
      }
    }
  }
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static inline fixed_s32 ABS(fixed_s32 x) { return (x < 0) ? -x : x; }

static inline HitboxComponent getOverlap(PositionComponent *posA,
                                         HitboxComponent *hitA,
                                         PositionComponent *posB,
                                         HitboxComponent *hitB) {
  int Ax1 = FIXED_TO_INT(posA->x);
  int Ay1 = FIXED_TO_INT(posA->y);
  int Bx1 = FIXED_TO_INT(posB->x);
  int By1 = FIXED_TO_INT(posB->y);

  int Ax2 = Ax1 + hitA->width;
  int Ay2 = Ay1 + hitA->height;
  int Bx2 = Bx1 + hitB->width;
  int By2 = By1 + hitB->height;

  HitboxComponent overlap = {.width = MIN(Ax2, Bx2) - MAX(Ax1, Bx1),
                             .height = MIN(Ay2, By2) - MAX(Ay1, By1)};
  return overlap;
};

// Collision resolution helpers
static inline void resolveStaticCollisions(PositionComponent *position,
                                           VelocityComponent *velocity,
                                           HitboxComponent overlap) {
  if (overlap.width < overlap.height) {
    if (velocity->dx > 0) {
      position->x -= INT_TO_FIXED(overlap.width);
    } else {
      position->x += INT_TO_FIXED(overlap.width);
    }
    velocity->dx = 0;
  } else {
    if (velocity->dy > 0) {
      position->y -= INT_TO_FIXED(overlap.height);
    } else {
      position->y += INT_TO_FIXED(overlap.height);
    }
    velocity->dy = 0;
  }
}

static inline void
resolveDynamicCollisions(Entity *entity, VelocityComponent *velA,
                         PositionComponent *posA, VelocityComponent *velB,
                         PositionComponent *posB, HitboxComponent overlap) {

  int resolveHorizontally;
  // compare entity priorities
  //

  if (entity->flag & (HORIZONTAL_COLLISION | VERTICAL_COLLISION)) {
    resolveHorizontally = (entity->flag & HORIZONTAL_COLLISION);
  } else {
    resolveHorizontally = (overlap.width < overlap.height);
    entity->flag |=
        resolveHorizontally ? HORIZONTAL_COLLISION : VERTICAL_COLLISION;
  }
  if (resolveHorizontally) {
    int speedA = FIXED_TO_INT(ABS(velA->dx));
    int speedB = FIXED_TO_INT(ABS(velB->dx));
    int totalSpeed = speedA + speedB;
    if (totalSpeed > 0) {
      int moveA = (overlap.width * speedA) / totalSpeed;
      int moveB = overlap.width - moveA;
      if (posA->x < posB->x) {
        posA->x -= INT_TO_FIXED(moveA);
        posB->x += INT_TO_FIXED(moveB);
      } else {
        posA->x += INT_TO_FIXED(moveA);
        posB->x -= INT_TO_FIXED(moveB);
      }
    }
  } else {
    int speedA = FIXED_TO_INT(ABS(velA->dy));
    int speedB = FIXED_TO_INT(ABS(velB->dy));
    int totalSpeed = speedA + speedB;
    if (totalSpeed > 0) {
      int moveA = (overlap.height * speedA) / totalSpeed;
      int moveB = overlap.height - moveA;
      if (posA->y < posB->y) {
        posA->y -= INT_TO_FIXED(moveA);
        posB->y += INT_TO_FIXED(moveB);
      } else {
        posA->y += INT_TO_FIXED(moveA);
        posB->y -= INT_TO_FIXED(moveB);
      }
    }
  }
}

static inline int determineCollisionType(PositionComponent *posA,
                                         VelocityComponent *velA,
                                         PositionComponent *posB,
                                         HitboxComponent *hitboxA,
                                         HitboxComponent *hitboxB) {

  // Get centers of both entities
  fixed_s32 centerAx = posA->x + INT_TO_FIXED(hitboxA->width >> 1);
  fixed_s32 centerAy = posA->y + INT_TO_FIXED(hitboxA->height >> 1);
  fixed_s32 centerBx = posB->x + INT_TO_FIXED(hitboxB->width >> 1);
  fixed_s32 centerBy = posB->y + INT_TO_FIXED(hitboxB->height >> 1);

  // Get direction vector between centers
  fixed_s32 dx = centerBx - centerAx;
  fixed_s32 dy = centerBy - centerAy;

  int collisionFlags = 0;

  // Check horizontal collision
  if ((dx > 0 && velA->dx > 0) || (dx < 0 && velA->dx < 0)) {
    collisionFlags |= HORIZONTAL_COLLISION;
  }

  // Check vertical collision
  if ((dy > 0 && velA->dy > 0) || (dy < 0 && velA->dy < 0)) {
    collisionFlags |= VERTICAL_COLLISION;
  }

  // If no clear direction from velocity, use position
  if (collisionFlags == 0) {
    if (ABS(dx) > 0)
      collisionFlags |= HORIZONTAL_COLLISION;
    if (ABS(dy) > 0)
      collisionFlags |= VERTICAL_COLLISION;
  }

  return collisionFlags;
}

// goal for next commit:  implement single jump with on ground flag.
// on input handler, only accept jump input if on ground flag is active
// in input handler jump case, set velocity AND clear on ground flag
// upon collision with GROUND ONLY, set on ground flag

void updateCollisionSystem(Entity *entity, PositionComponent *position,
                           VelocityComponent *velocity, HitboxComponent *hitbox,
                           fixed_s32 deltaTime) {
  for (int idA = 0; idA < MAX_ENTITIES; idA++) {
    if (!(entity[idA].flag & ACTIVE)) 
      continue;
    if (!(entity[idA].flag & DETECTS_COLLISIONS))
      continue;

    // int hasCollision = 0; // track if collision this frame
    // Clear previous collision flags at start of frame
    entity[idA].flag &=
        ~(HORIZONTAL_COLLISION | VERTICAL_COLLISION);


    for (int idB = idA + 1; idB < MAX_ENTITIES; idB++) {
      if (!(entity[idB].flag & ACTIVE)) 
        continue;
      if (!(entity[idB].flag & TRIGGERS_COLLISIONS))
        continue; 

      HitboxComponent overlap = getOverlap(&position[idA], &hitbox[idA],
                                           &position[idB], &hitbox[idB]);

      if (overlap.width < 0 || overlap.height < 0) {
        continue;
      }

      int collisionFlag =
          determineCollisionType(&position[idA], &velocity[idA], &position[idB],
                                 &hitbox[idA], &hitbox[idB]);

      entity[idA].flag |= collisionFlag;

      if (idA == 0 && (collisionFlag & VERTICAL_COLLISION) &&
          (velocity[idA].dy > 0) && (entity[idB].flag & IS_GROUND) &&
          (position[idA].y < position[idB].y)) {
        entity[idA].flag |= ON_GROUND;
      }

      if (entity[idB].flag & STATIC_COLLIDER) {
        resolveStaticCollisions(&position[idA], &velocity[idA], overlap);
      } else { // dynamic collision handling
        resolveDynamicCollisions(&entity[idA], &velocity[idA], &position[idA],
                                 &velocity[idB], &position[idB], overlap);
      }
    } 
  }
}

void updateAnimationSystem(Entity *entity, AnimationComponent *animation) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (!(entity[id].flag & ACTIVE)) continue;
    if (entity[id].flag & ANIMATION_COMPONENT) {
      animation[id].keyframe++;
      if (animation[id].keyframe >= animation[id].keyframeInterval) {
        entity[id].flag |= DIRTY;
        animation[id].keyframe = 0;
        animation[id].frameNumber++;
        if (animation[id].frameNumber == 4) {
          animation[id].frameNumber = 0;
        }
      }
    }
  }
}

// todo: optimize clearing of text, contemplate where this is handled
void updateRenderSystem(ECS *ecs, Entity *entity, AnimationComponent *animation,
                        DrawingComponent *draw, TextComponent *text, const void *image) {
  for (int id = 0; id < MAX_ENTITIES; id++) {
    if (!(entity[id].flag & DIRTY)) continue;
    if (!(entity[id].flag & ACTIVE)) continue;
    
    if (entity[id].flag & SPRITE_FLAG) {
      clearSpriteFrame(ecs->components->position[id].prevX, ecs->components->position[id].prevY, 32, image);

      renderEntity(ecs, id);
    }
    if (entity[id].flag & DRAWING_COMPONENT) {
      draw[id].drawingRoutine(ecs, id);
    }
    if (entity[id].flag & TEXT_COMPONENT) {
      // todo: handle text clearing?  only necessary for changing text, like dialog. can be simple as clearing monocolor text box
      renderEntityText(ecs, id);
    }

    entity[id].flag &= ~DIRTY;
  }
};
