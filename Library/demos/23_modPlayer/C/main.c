#include "../data/dataSndData.h"
#include "audio/mixer.h"
#include "audio/modPlayer.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"

#define DSPS (*(volatile u16 *)0x04000004)

int main() {
  DSPC = MODE3 | BG2;

  initCharLookup();
  initializeFIFOInterrupts();
  modInit();
  while (1) {
    updateKeys();

    if (reload == 1) {
      modUpdate();
      reload = 0;
    }

    if (keyTapped(B)) {
      playMod(MOD_BIT_Introtune);
    }
  }

  return 0;
}
