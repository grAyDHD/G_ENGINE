#include "../data/dataSndData.h"
#include "audio/audio.h"
#include "audio/mixer.h"
#include "audio/modPlayer.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"

int main() {
  DSPC = MODE3 | BG2;

  initCharLookup();
  initializeFIFOInterrupts();
  SndInit();
  gprintf(5, 5, "mixFreq: %d", modTiming.mixFreq);
  gprintf(5, 15, "rcpMixFreq: %d", modTiming.rcpMixFreq);

  while (1) {
    //    VBLANK();

    updateKeys();

    if (reload == 1) {
      SndUpdate();
      reload = 0;
    }

    if (keyTapped(B)) {
      SndPlayMOD(MOD_);
    }
  }

  return 0;
}
