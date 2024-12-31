#include "../includes/characterAnimator.h"
#include "../build/Robo.h"
#include "Bedroom.h"
#include "graphics/video.h"
#include "input/in.h"

static ComponentManager world;
static Entity entities[MAX_ENTITIES];
static EntitySystem entitySystem;

int main() {
  DSPC = MODE3 | BG2;
  initEntitySystem(&entitySystem, &world);
  int playerId = createPlayer(&entitySystem, &RoboBitmap);

  while (1) {
    VBLANK();
    updateKeys();

    updateInputSystem(&entitySystem, &world);
    clearSpriteFrame(world.position[playerId].x, world.position[playerId].y, 32,
                     BedroomBitmap);
    renderPlayer(&entitySystem, playerId);
  }

  return 0;
}
