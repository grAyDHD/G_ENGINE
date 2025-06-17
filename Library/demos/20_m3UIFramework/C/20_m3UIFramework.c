#include "../includes/ui.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "ecs/entities.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"

static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime;

// Commits: create entity with pause behavior - player for simplicity???
// on pause, hardcode change in player coordinates off fillScreen// disable player input
// create entity for pause text with input to re enable player input and revert player coordinates

// Current commit: create text component and entity, and render text

int main() {
  DSPC = MODE3 | BG2;

  initializeVBI();

  fillScreen(0x0000);
  

  initEntitySystem(&ecs, &components);

  createPlayer(&ecs, SonicBitmap);
  ecs.entity[0].flag |= PHYSICS_FLAG;
  createScreenBorders(&ecs);

  int textEntityId = createEntity(&ecs, POSITION_COMPONENT | TEXT_COMPONENT);
  ecs.components->position[textEntityId].x = 20;
  ecs.components->position[textEntityId].y = 20;
  ecs.components->text[textEntityId].text = "hello";

  setTextColor(31, 31, 31);
//  gprintf(20, 20, "hello", 0);
  while (1) {
    VBLANK();
    fillScreen(0x0000);
    gprintf(20, 20, "hello", 0);


      updateInputSystem(&ecs, ecs.entity, ecs.components->input, deltaTime);
      updateBehaviorSystem(&ecs, ecs.entity, ecs.components->ai);
      updatePhysicsSystem(ecs.entity, ecs.components->velocity,
                        ecs.components->acceleration, deltaTime);
      updateMovementSystem(ecs.entity, ecs.components->position,
                         ecs.components->velocity, deltaTime);
      updateCollisionSystem(ecs.entity, ecs.components->position,
                          ecs.components->velocity, ecs.components->hitbox,
                          deltaTime);
      updateAnimationSystem(ecs.entity, ecs.components->animation);
      updateRenderSystem(&ecs, ecs.entity, ecs.components->animation,
                       ecs.components->draw, ecs.components->text);
      deltaTime = 0;
  }

  return 0;
}
