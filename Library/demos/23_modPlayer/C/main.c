#include "../data/dataSndData.h"
#include "audio/mixer.h"
#include "audio/modPlayer.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"

int main() {
  DSPC = MODE3 | BG2;

  initCharLookup();
  initializeFIFOInterrupts();
  modInit();
  gprintf(5, 5, "mixFreq: %d", modTiming.mixFreq);
  gprintf(5, 15, "rcpMixFreq: %d", modTiming.rcpMixFreq);

  while (1) {
    //    VBLANK();

    updateKeys();

    if (reload == 1) {
      modUpdate();
      reload = 0;
    }

    if (keyTapped(B)) {
      playMod(MOD_);
    }
  }

  return 0;
}
