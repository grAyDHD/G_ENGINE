#include "../includes/21_debug.h"

#include "ecs/systems.h"
#include "graphics/video.h"
#include "core/interrupts.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "graphics/draw.h"

#include "metalsonic.h"

static ComponentStorage components;
static ECS ecs;

volatile fixed_s32 deltaTime;

int main() {
  DSPC = MODE3 | BG2;
  initializeVBI();

//  m3_Background(const void *src)
  initEntitySystem(&ecs, &components);
  createPlayer(&ecs, metalsonicBitmap);

  createScreenBorders(&ecs);

  while (1) {
    VBLANK(); 
    updateRenderSystem(&ecs, ecs.entity, ecs.components->animation, ecs.components->draw);

  }
  return 0;
}
