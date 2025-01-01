#include "../includes/characterAnimator.h"
#include "../build/Robo.h"
#include "Bedroom.h"
#include "core/timer.h"
#include "graphics/video.h"
#include "input/in.h"

static ComponentManager world;
static Entity entities[MAX_ENTITIES];
static EntitySystem entitySystem;

int main() {
  DSPC = MODE3 | BG2;
  initEntitySystem(&entitySystem, &world);
  int playerId = createPlayer(&entitySystem, &RoboBitmap);
  createNPC(&entitySystem, &RoboBitmap);

  m3_Background(BedroomBitmap);
  while (1) {
    VBLANK();

    clearSpriteFrame(world.position[playerId].x, world.position[playerId].y, 32,
                     BedroomBitmap);
    clearSpriteFrame(world.position[playerId + 1].x, world.position[playerId + 1].y, 32, BedroomBitmap);
    updateInputSystem(&entitySystem, &world);
    updateBehaviorSystem(&entitySystem, &world);

    
    updateRenderSystem(&entitySystem, &world);

    simpleWait(10);
  }

  return 0;
}
