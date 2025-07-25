#include "../includes/23_modPlayer.h"
#include "core/interrupts.h"
#include "core/timer.h"
#include "graphics/m3Text.h"
#include "input/in.h"

void initializeInterrupts() {
  ISR = SndVSync;
  DISPSTAT = 1 << 3; // tell display to fire vblank interrupts
  irqEnable(IRQ_VBLANK);
  irqMaster(ON);
}

int main() {
  DSPC = MODE3 | BG2;

  initCharLookup();
  initializeInterrupts();
  SndInit(SND_FREQ_18157);

  while (1) {
    VBLANK();

    updateKeys();

    if (keyTapped(B)) {
      SndPlayMOD(MOD_);
    }

    SndUpdate();
  }

  return 0;
}
