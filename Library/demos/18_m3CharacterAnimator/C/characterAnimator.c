#include "../includes/characterAnimator.h"
#include "Bedroom.h"
#include "graphics/video.h"
#include "input/in.h"

static ComponentManager world;
static Entity entities[MAX_ENTITIES];
static int nextEntityId = 0;

int createEntity(int componentMask);
int createPlayer(const void *spriteSheet);
void renderPlayer(int playerId, ComponentManager *world);

int main() {
  DSPC = MODE3 | BG2;
  int playerId = createPlayer(&RoboBitmap);

  while (1) {
    updateKeys();
    VBLANK();

    clearSpriteFrame(world.position[playerId].x, world.position[playerId].y, 32,
                     BedroomBitmap);

    // set state/direction
    //  check for collision, if none move position
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

    renderPlayer(playerId, &world);

    if (keyReleased(U) | keyReleased(D) | keyReleased(L) | keyReleased(R)) {
      world.animation[playerId].frameNumber = 0;
      world.animation[playerId].state = IDLE;
    }
  }

  return 0;
}

int createEntity(int componentMask) {
  if (nextEntityId >= MAX_ENTITIES) {
    return -1;
  }

  Entity *entity = &entities[nextEntityId];
  entity->entityID = nextEntityId;
  entity->componentMask = componentMask;

  if (componentMask & COMPONENT_POSITION) {
    world.position[nextEntityId] = (PositionComponent){120, 120};
  }

  if (componentMask & COMPONENT_VELOCITY) {
    world.velocity[nextEntityId] = (VelocityComponent){0, 0};
  }

  return nextEntityId++;
}

int createPlayer(const void *spriteSheet) {

  int playerId = createEntity(PLAYER_ENTITY);

  world.animation[playerId] =
      (AnimationComponent){.frameNumber = 1, .direction = LEFT, .state = WALK};
  world.sprite[playerId] = (SpriteComponent){.spriteSheet = spriteSheet};

  return playerId;
}

void renderPlayer(int playerId, ComponentManager *world) {
  SpriteFrame32Bit(&world->position[playerId], &world->animation[playerId],
                   world->sprite[playerId].spriteSheet);
}
