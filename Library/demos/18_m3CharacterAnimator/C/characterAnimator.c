#include "../includes/ECS.h"
#include "Bedroom.h"
#include "graphics/video.h"
#include "input/in.h"

static ComponentManager world;
static Entity entities[MAX_ENTITIES];
static EntitySystem entitySystem;

int main() {
  DSPC = MODE3 | BG2;
  initEntitySystem(&entitySystem, &world, entities, MAX_ENTITIES);
  int playerId = createPlayer(&entitySystem, &RoboBitmap);

  while (1) {
    updateKeys();
    VBLANK();

    //   clearSpriteFrame(world.position[playerId].x,
    //   world.position[playerId].y, 32,
    //                  BedroomBitmap);

    // set state/direction
    //  check for collision, if none move position

    /*
    if (keyDown(U)) {
      world.position[playerId].y -= 1;
      world.animation[playerId].state = WALK;
      world.animation[playerId].direction = UP;
    } else if (keyDown(D)) {
      world.position[playerId].y += 1;
      world.animation[playerId].state = WALK;
      world.animation[playerId].direction = DOWN;
    } else if (keyDown(L)) {
      world.position[playerId].x -= 1;
      world.animation[playerId].state = WALK;
      world.animation[playerId].direction = LEFT;
    } else if (keyDown(R)) {
      world.position[playerId].x += 1;
      world.animation[playerId].state = WALK;
      world.animation[playerId].direction = RIGHT;
    }
    */

    renderPlayer(&entitySystem, playerId);

    //   if (keyReleased(U) | keyReleased(D) | keyReleased(L) | keyReleased(R))
    //   {
    //    world.animation[playerId].frameNumber = 0;
    //    world.animation[playerId].state = IDLE;
    // }
  }

  return 0;
}
