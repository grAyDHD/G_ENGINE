#include "../includes/ui.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "ecs/entities.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"
#include "input/in.h"

typedef struct {
    char* text;
    int x, y;           // Position
    int flags;          // PAUSE_UI | MENU_UI, etc.
} MenuItem;

typedef enum { PLAYING, PAUSED } GameState;

static ComponentStorage components;
static ECS ecs;
static GameState gameState = PLAYING;

volatile fixed_s32 deltaTime;

// Next commit: incorporate clearSpriteFrame, 16 or 32?  Not now, but eventually add case handling that checks sprite sizes
void createPauseMenu(ECS *ecs) {
  MenuItem pauseMenuItems[] = {
    {"Select Character",  80, 60, PAUSE_UI | MENU_UI | DIRTY},
    {"Movement Speed",    80, 80, PAUSE_UI | MENU_UI},
    {"Text Speed",        80, 100, PAUSE_UI | MENU_UI},
    {"Invert Green",      80, 120, PAUSE_UI | MENU_UI}
  };

// Generate entities from data
  for (int i = 0; i < 4; i++) {
    int id = createEntity(ecs, POSITION_COMPONENT | TEXT_COMPONENT | pauseMenuItems[i].flags);
    ecs->components->position[id].x = pauseMenuItems[i].x;
    ecs->components->position[id].y = pauseMenuItems[i].y;
    ecs->components->text[id].text = pauseMenuItems[i].text;
  }
}

void pauseGameState(ECS *ecs) {
  for (int i = 0; i < 10; i++) {
    if (ecs->entity[i].flag & GAME_ENTITY) {
      ecs->entity[i].flag &= ~ACTIVE; 
    }
    if (ecs->entity[i].flag & PAUSE_UI) {
      ecs->entity[i].flag |= ACTIVE; 
    }
  }
  gameState = PAUSED;
}

void resumeGameState(ECS *ecs) {
  for (int i = 0; i < 10; i++) {
    if (ecs->entity[i].flag & GAME_ENTITY) {
      ecs->entity[i].flag |= ACTIVE; 
    }
    if (ecs->entity[i].flag & PAUSE_UI) {
      ecs->entity[i].flag &= ~ACTIVE; 
    }
  }
  gameState = PLAYING;

}

void globalInputHandler(ECS *ecs, int entityId) {
  if (keyTapped(ST)) {
    switch (gameState) {
      case PLAYING:
        pauseGameState(ecs); break;
      case PAUSED:
        resumeGameState(ecs);
        break;
    }
  }
}

int createGlobalInputEntity(ECS *ecs) {
  int id = createEntity(ecs, INPUT_COMPONENT | ACTIVE);
  ecs->components->input[id].handleInput = globalInputHandler;  
  return id;
}

int main() {
  DSPC = MODE3 | BG2;
  initializeVBI();
  initEntitySystem(&ecs, &components);

  createPlayer(&ecs, SonicBitmap);
  createGlobalInputEntity(&ecs);
  createScreenBorders(&ecs);
//  createPauseMenu(&ecs);

  int textEntityId = createEntity(&ecs, POSITION_COMPONENT | TEXT_COMPONENT | PAUSE_UI);
  ecs.components->position[textEntityId].x = 100;
  ecs.components->position[textEntityId].y = 100;
  ecs.components->text[textEntityId].text = "PAUSED";

// todo: have rendering of text from text components handle setting color  and setting it back to original color before and after text render
  setTextColor(31, 31, 31);

  while (1) {
    VBLANK();
    fillScreen(0x0000);

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
