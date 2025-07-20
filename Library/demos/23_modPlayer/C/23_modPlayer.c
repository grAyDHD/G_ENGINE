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
void initDebugDisplay() {
  // Draw static labels once at startup - never cleared
  printString(10, 10, "Pattern:");
  printString(10, 22, "Row:");
  printString(10, 34, "Tempo:");
  // etc.
}

void updateDebugNumbers(int pattern, int row, int tempo) {
  // Only clear/redraw the number portions
  clearTextArea(70, 10, 30, 11); // Clear just the number area
  gprintf(70, 10, "%d", pattern);

  clearTextArea(70, 22, 30, 11);
  gprintf(70, 22, "%d", row);

  clearTextArea(70, 34, 30, 11);
  gprintf(70, 34, "%d", tempo);
}
void updateDebugDisplay(int pattern, int row) {
  clearTextArea(10, 10, 80, 8);
  gprintf(10, 10, "Pattern: %d", pattern);

  clearTextArea(10, 20, 60, 8);
  gprintf(10, 20, "Row: %d", row);
}

int main() {
  DSPC = MODE3 | BG2;

  initCharLookup();
  initMixChannels();

  irqMaster(ON);  // now enable interrupts
  initMonoFIFO(); // is now reading from buffer

  initDebugDisplay();

  int x = 0, y = 0;

  while (1) {
    if (reload == 1) {
      mixAudio();
      reload = 0;
    }

    x++;
    y += 2;

    VBLANK();
    updateDebugNumbers(x, y, 0);
  }
  return 0;
}
