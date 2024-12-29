#include "../includes/characterAnimator.h"
#include "Bedroom.h"
#include "graphics/video.h"

static ComponentManager world;
static Entity entities[MAX_ENTITIES];
static int nextEntityId = 0;

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
      (AnimationComponent){.frameNumber = 0, .direction = DOWN, .state = IDLE};
  world.sprite[playerId] = (SpriteComponent){.spriteSheet = spriteSheet};

  return playerId;
}

void renderPlayer(int playerId, ComponentManager *world) {
  SpriteFrame32Bit(&world->position[playerId], &world->animation[playerId],
                   world->sprite[playerId].spriteSheet);
}

int main() {
  DSPC = MODE3 | BG2;
  int playerId = createPlayer(&RoboSmallBitmap);

  while (1) {
    VBLANK();
    renderPlayer(playerId, &world);
  }
  return 0;
}
