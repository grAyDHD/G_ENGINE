#include "../includes/characterAnimator.h"
#include "Bedroom.h"
#include "graphics/video.h"

static ComponentManager components;
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
    components.position[nextEntityId] = (PositionComponent){120, 120};
  }

  if (componentMask & COMPONENT_VELOCITY) {
    components.velocity[nextEntityId] = (VelocityComponent){0, 0};
  }

  return nextEntityId++;
}

int createPlayer(const void *spriteSheet) {

  int playerId = createEntity(PLAYER_ENTITY);

  components.animation[playerId] =
      (AnimationComponent){.frameCount = 0, .state = IDLE, .direction = DOWN};
  components.sprite[playerId] =
      (SpriteComponent){.spriteSheet = &spriteSheet, .width = 32, .height = 32};

  return playerId;
}

void renderingSystem(ComponentManager components) {
  for (int i = 0; i < nextEntityId; i++) {
    SpriteFrame32Bit(components.position[i].x, components.position[i].y,
                     components.animation[i].frameCount,
                     components.sprite[i].spriteSheet, 3);
  }
}

void playerInputSystem() {
  // check for user input and update player entity
}

int main() {
  DSPC = MODE3 | BG2;
  int playerId = createPlayer(&PkmnSlvrGirlWalkingBitmap);

  m3_Background(BedroomBitmap);
  while (1) {
  }
  return 0;
}

/*

HOUSE BINGO
their liver is failing
house called an ass
house hits with cane
patient fakeout opening scene
house eats wilson's food
sarcoidosis
vasculitis
stop unnecessary procedure at last second
lumbara puncture
std in the clinic
everyone gives house different advice
lupus
blood coming frome somewhere it shouldn't
breaking and entering
the patient is crashing
cameron eye roll
house proven wrong
introspective house
hose has his idea face
all pagers go off at same time
inside body animation
drill into patients brain
house takes vicodin
patient freaks out in mri
intubation
stroke
house playing with cane
*/
