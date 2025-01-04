#include "../includes/characterAnimator.h"
#include "ecs/systems.h"

static ComponentStorage world;
static ECS entitySystem;

int main() {
  DSPC = MODE3 | BG2;
  initEntitySystem(&entitySystem, &world);
  createPlayer(&entitySystem, &RoboBitmap); // returns ID
  createNPC(&entitySystem, &RoboBitmap);    // returns ID

  m3_Background(BedroomBitmap);
  while (1) {
    VBLANK();

    clearSpriteFrames(&entitySystem, &world, BedroomBitmap);

    updateInputSystem(&entitySystem, &world);
    //    updateBehaviorSystem(&entitySystem, &world);
    updateCollisionSystem(&entitySystem, &world);
    updateMovementSystem(&entitySystem, &world);
    updateAnimationSystem(&entitySystem, &world);
    updateRenderSystem(&entitySystem, &world);
  }

  return 0;
}

/*
while (1) {
    VBLANK();

    clearSpriteFrames(&entitySystem, &world, BedroomBitmap);

    updateInputSystem(&entitySystem, &world);
    updateBehaviorSystem(&entitySystem, &world);
    updatePhysicsSystem(&entitySystem, &world); // Apply gravity
    updateMovementSystem(&entitySystem, &world); // Predictive movement &
collision updateRenderSystem(&entitySystem, &world);
}
*/
