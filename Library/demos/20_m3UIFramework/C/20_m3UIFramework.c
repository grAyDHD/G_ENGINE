#include "../includes/ui.h"
#include "../build/Bedroom.h"
#include "../build/Robo.h"
#include "input/in.h"
#include "ecs/entities.h"
#include "graphics/draw.h"
#include "input/in.h"

// Commits:
// must implement text functions to change color of text

static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime;

typedef enum {
  PLAYING,
  PAUSE_MENU
} GameState;

typedef enum {
  VOLUME,
  CHARACTER_SELECTION,
  // etc.

} MenuState;

int main() {
  DSPC = MODE3 | BG2;
  initializeVBI();
  fillScreen(0x0000);
  initEntitySystem(&ecs, &components);
  createPlayer(&ecs, SonicBitmap);
  ecs.entity[0].flag |= PHYSICS_FLAG; //ON_GROUND;
  createScreenBorders(&ecs);

  GameState state = PLAYING;

  while (1) {
    VBLANK();
    fillScreen(0x0000);


    if(keyTapped(ST)) {
//      ecs.entity[0].flag &= ~ENABLE_INPUT;
      state = PAUSE_MENU;
      // if game PLAYING, set state to PAUSE_MENU  
      //   disable player input by removing INPUT_ENABLED flag
      // else if game state PAUSE_MENU, set state to PLAYING
      //   enable player input again 

    }

    //  Main PLAYING loop below
    switch(state) {
      case(PLAYING):
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
                       ecs.components->draw);
      deltaTime = 0;
    }
  }

  return 0;
}
