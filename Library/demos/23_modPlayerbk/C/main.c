#include "../includes/23_modPlayer.h"
#include "core/interrupts.h"
#include "core/timer.h"
#include "graphics/m3Text.h"
#include "input/in.h"

static int vblankCounter = 0;

void SndVSync() {
  vblankCounter++;
  if (sndVars.activeBuffer == 1) // buffer 1 just got over
  {
    // Start playing buffer 0
    REG_DM1CNT = 0;
    REG_DM1SAD = (u32)sndVars.mixBufferBase;
    REG_DM1CNT_H =
        DMA_DEST_FIXED | DMA_REPEAT | DMA_WORD | DMA_MODE_FIFO | DMA_ENABLE;

    // Set the current buffer pointer to the start of buffer 1
    sndVars.curMixBuffer = sndVars.mixBufferBase + sndVars.mixBufferSize;
    sndVars.activeBuffer = 0;
  } else // buffer 0 just got over
  {
    // DMA points to buffer 1 already, so don't bother stopping and resetting it

    // Set the current buffer pointer to the start of buffer 0
    sndVars.curMixBuffer = sndVars.mixBufferBase;
    sndVars.activeBuffer = 1;
  }
  irqAcknowledge(IRQ_VBLANK);
}

int main() {
  //  s32 i;

  DSPC = MODE3 | BG2;

  initCharLookup();

  printString(5, 5, "Counter; ");
  printString(5, 15, "B button counter; ");
  printString(5, 25, "Sound init; ");
  printString(5, 45, "MOD state: ");
  printString(5, 55, "MOD orders: ");
  printString(5, 65, "Ch0 data: ");
  printString(5, 75, "Ch0 volume: ");
  printString(5, 85, "Mix buffer: ");

  ISR = SndVSync;

  DISPSTAT = 1 << 3; // tell display to fire vblank interrupts
  irqEnable(IRQ_VBLANK);

  SndInit(SND_FREQ_18157);
  gprintf(100, 25, "%d", sndVars.mixFreq);

  irqMaster(ON);

  static int bCounter = 0;
  while (1) {
    VBLANK();

    updateKeys();

    if (keyTapped(B)) {
      SndPlayMOD(MOD_AdventureBoy);
      bCounter++;
    }

    SndUpdate();
  }

  return 0;
}

/*
// Counter:
clearTextArea(70, 5, 50, 10);
gprintf(70, 5, "%d", vblankCounter);

// B button counter
clearTextArea(102, 15, 20, 10);
gprintf(102, 15, "%d", bCounter);

// Show MOD state
clearTextArea(102, 45, 20, 10);
gprintf(102, 45, "%d", sndMod.state);
clearTextArea(102, 55, 20, 10);
gprintf(102, 55, "%d", sndMod.orderCount);

// And in your main loop, add these displays:
clearTextArea(102, 65, 100, 10);
gprintf(102, 65, "%x", (u32)sndChannel[0].data);
clearTextArea(102, 75, 20, 10);
gprintf(102, 75, "%d", sndChannel[0].vol);

// buffer filling?
clearTextArea(102, 85, 40, 10);
gprintf(102, 85, "%x", (u32)sndVars.curMixBuffer);
*/
