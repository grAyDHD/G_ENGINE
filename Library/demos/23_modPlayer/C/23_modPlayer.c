#include "../includes/23_modPlayer.h"
#include "audio/mixer.h"
#include "core/interrupts.h"
#include "graphics/video.h"

int main() {
  DSPC = MODE3 | BG2;

  initMixChannels();

  irqMaster(ON);  // now enable interrupts
  initMonoFIFO(); // is now reading from buffer

  while (1) {
    if (reload == 1) {
      mixAudio();
      reload = 0;
    }
  }
  return 0;
}
