#include "../includes/ui.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "ecs/entities.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"

static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime;

// Next commit: Flag for entity type: GAME_ENTITY PAUSE_UI HUD_UI MENU_UI 
// Following commit: add start button behavior to call pause/resume game state.  Player retains it's own input, but it's input is not checked if the player entity is not active, such as when paused.  I may need to additionally have a check for INPUT_DISABLED or something similar for cutscenes that use the player sprite/entity but dont want player interference with scripted behavior

void pauseGameState(ECS *ecs) {
  // disableGameEntities
  // enablePauseEntities
}

void resumeGameState(ECS *ecs) {
  // enableGameEntities
  // disablePauseEntities

}

int main() {
  DSPC = MODE3 | BG2;
  initializeVBI();
  initEntitySystem(&ecs, &components);

  createPlayer(&ecs, SonicBitmap);
  createScreenBorders(&ecs);

  int textEntityId = createEntity(&ecs, POSITION_COMPONENT | TEXT_COMPONENT);
  ecs.components->position[textEntityId].x = 100;
  ecs.components->position[textEntityId].y = 100;
  ecs.components->text[textEntityId].text = "goodbye";

  // todo: have rendering of text from text components handle setting color  and setting it back to original color before and after text render
  setTextColor(31, 31, 31);

  while (1) {
    VBLANK();
    fillScreen(0x0000);

  //int x = ecs.components->position[5].x;
  //int y = ecs.components->position[5].y;
  //gprintf(x, y, ecs.components->text[5].text, 0);


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
