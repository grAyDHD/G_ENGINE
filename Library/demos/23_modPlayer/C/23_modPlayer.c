#include "../includes/23_modPlayer.h"
#include "audio/mixer.h"
#include "core/interrupts.h"
#include "core/timer.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"

// samplesPerTick = mixFreq / MODFreq
// MODFreq = tempo in Hz, where hz = tempo * 2/5
// so, if tempo equals 120 bpm:
// 120/(2/5) = 300
// mixFreq = sampleRate = 16000
// samplesPerTick = 16000/300 = 53.333

void updateDebugDisplay(int pattern, int row) {
  clearTextArea(10, 10, 80, 8);
  gprintf(10, 10, "Pattern: %d", pattern);

  clearTextArea(10, 20, 60, 8);
  gprintf(10, 20, "Row: %d", row);
}

int main() {
  DSPC = MODE3 | BG2;

  initMixChannels();

  irqMaster(ON);  // now enable interrupts
  initMonoFIFO(); // is now reading from buffer

  int x = 0, y = 0;

  while (1) {
    if (reload == 1) {
      mixAudio();
      reload = 0;
    }

    x++;
    y += 2;

    updateDebugDisplay(x, y);
    VBLANK();
  }
  return 0;
}
