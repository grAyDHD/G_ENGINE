#include "../includes/characterAnimator.h"

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
    updateBehaviorSystem(&entitySystem, &world);
    updateMovementSystem(&entitySystem, &world);
    updateRenderSystem(&entitySystem, &world);
  }

  return 0;
}
