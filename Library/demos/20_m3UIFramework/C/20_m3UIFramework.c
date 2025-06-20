#include "../includes/ui.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "ecs/entities.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"
#include "input/in.h"

// next commit: refactor bit flags. then, add UIComponent, and add the checkbox first, as it will be the easisest UI element to make functional.
// this will require UI rendering routines.  Therefore, first commit is flag refactor, and contemplate from there
typedef struct {
    char* text;
    int x, y;           // Position
    int flags;          // PAUSE_UI | MENU_UI, etc.
} MenuItem;

typedef enum { PLAYING, PAUSED } GameState;

static ComponentStorage components;
static ECS ecs;
static GameState gameState = PLAYING;

static int pauseMenuEntityIds[4];
static int selectedMenuIndex = 0;

volatile fixed_s32 deltaTime;

void createPauseMenu(ECS *ecs) {
  MenuItem pauseMenuItems[] = {
    {"Select Character",  80, 60, PAUSE_UI | MENU_UI | SELECTED | DIRTY},
    {"Movement Speed",    80, 80, PAUSE_UI | MENU_UI | DIRTY},
    {"Text Speed",        80, 100, PAUSE_UI | MENU_UI | DIRTY},
    {"Invert Green",      80, 120, PAUSE_UI | MENU_UI | DIRTY}
  };

// Generate entities from data
  for (int i = 0; i < 4; i++) {
    pauseMenuEntityIds[i] = createEntity(ecs, POSITION_COMPONENT | TEXT_COMPONENT | pauseMenuItems[i].flags);
    int id = pauseMenuEntityIds[i];
    ecs->components->position[id].x = pauseMenuItems[i].x;
    ecs->components->position[id].y = pauseMenuItems[i].y;
    ecs->components->text[id].text = pauseMenuItems[i].text;
    if (ecs->entity[id].flag & SELECTED) {
      ecs->components->text[id].color = RGB(0, 0, 0); 
    } else {
      ecs->components->text[id].color = RGB(31, 31, 31);
    }
  }
}

void pauseGameState(ECS *ecs) {
  for (int i = 0; i < 10; i++) {
    if (ecs->entity[i].flag & GAME_ENTITY) {
      ecs->entity[i].flag &= ~ACTIVE; 
    }
    if (ecs->entity[i].flag & PAUSE_UI) {
      ecs->entity[i].flag |= ACTIVE | DIRTY; 
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
  m3_Background(genericBitmapBitmap);

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

  if (gameState == PAUSED) {
    if (keyTapped(D) && selectedMenuIndex < 3) {
      int currentEntityId = pauseMenuEntityIds[selectedMenuIndex];

      ecs->entity[currentEntityId].flag &= ~SELECTED;
      ecs->entity[currentEntityId].flag |= DIRTY;
      ecs->components->text[currentEntityId].color = RGB(31, 31, 31);
      selectedMenuIndex++;

      int newEntityId = pauseMenuEntityIds[selectedMenuIndex];
      ecs->entity[newEntityId].flag |= SELECTED | DIRTY;
      ecs->components->text[newEntityId].color = RGB(0, 0, 0);
    }

    if (keyTapped(U) && selectedMenuIndex > 0) {
      int currentEntityId = pauseMenuEntityIds[selectedMenuIndex];

      ecs->entity[currentEntityId].flag &= ~SELECTED;
      ecs->entity[currentEntityId].flag |= DIRTY;
      ecs->components->text[currentEntityId].color = RGB(31, 31, 31);
      selectedMenuIndex--;

      int newEntityId = pauseMenuEntityIds[selectedMenuIndex];
      ecs->entity[newEntityId].flag |= SELECTED | DIRTY;
      ecs->components->text[newEntityId].color = RGB(0, 0, 0);

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

  m3_Background(genericBitmapBitmap);

  createPlayer(&ecs, SonicBitmap);
  createGlobalInputEntity(&ecs);
  createScreenBorders(&ecs);
  createPauseMenu(&ecs);

//  int textEntityId = createEntity(&ecs, POSITION_COMPONENT// | TEXT_COMPONENT | PAUSE_UI);
//  ecs.components->position[textEntityId].x = 100;
//  ecs.components->position[textEntityId].y = 100;
//  ecs.components->text[textEntityId].text = "PAUSED";

  while (1) {
    VBLANK();

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
                       ecs.components->draw, ecs.components->text, &genericBitmapBitmap);

      deltaTime = 0;

  }
  return 0;
}
