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

    // refactor into refreshRenderSystem(&entitySystem);
    clearSpriteFrame(world.position[playerId].x, world.position[playerId].y, 32,
                     BedroomBitmap);
    clearSpriteFrame(world.position[playerId + 1].x, world.position[playerId + 1].y, 32, BedroomBitmap);

    // update following 2 to update velocity rather than position, v system will handle movement
    updateInputSystem(&entitySystem, &world); // updates player position along with other input entities various params as long as INPUT_ENABLED 
    updateBehaviorSystem(&entitySystem, &world); // updates entities with AI
    //  should collisions be checked in player input function, and ai behavior functions?
    // annoyting part, hitbox size. I'll add this later cuz fuck that lolz
    // updateMovementSystem(&entitySystem);
    
    updateRenderSystem(&entitySystem, &world);

    simpleWait(10);
  }

  return 0;
}
