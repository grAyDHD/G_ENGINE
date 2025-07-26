#include "../includes/23_modPlayer.h"
#include "audio/mixer.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "core/timer.h"
#include "graphics/m3Text.h"
#include "input/in.h"

volatile u32 reloads = 0;
static volatile u32 fifoCounters = 0;

void dmaAudioIsr(void) {
  fifoCounters++;

  if (fifoCounters >= 16) { // increment every 16 samples
    fifoCounters = 0;
    reloads = 1;
    if (mbuf.activeBuffer == bufB) {
      // begin streaming to FIFO from buffer base
      DMA[1].control = 0;
      DMA[1].source = (u32)mbuf.bufBase;
      DMA[1].control = DMA_DEST_FIXED | DMA_REPEAT | DMA_32BIT |
                       DMA_START_SPECIAL | DMA_ENABLE | DMA_IRQ_ENABLE;

      mbuf.activeBuffer = bufA;
      mbuf.position =
          mbuf.bufBase + BUFFER_SIZE; // sets write position to inactive buffer
    } else                            // buffer 1 just got over
    {
      mbuf.activeBuffer = bufB;
      mbuf.position = mbuf.bufBase;
    }
  }

  irqAcknowledge(IRQ_DMA1);
}

void initializeInterrupts() {
  ISR = dmaAudioIsr;
  irqEnable(IRQ_DMA1);
  irqMaster(ON);
}

int main() {
  DSPC = MODE3 | BG2;

  initCharLookup();
  initializeInterrupts();
  SndInit();
  gprintf(5, 5, "mixFreq: %d", modTiming.mixFreq);
  gprintf(5, 15, "rcpMixFreq: %d", modTiming.rcpMixFreq);

  while (1) {
    //    VBLANK();

    updateKeys();

    if (reloads == 1) {
      SndUpdate();
      reloads = 0;
    }

    if (keyTapped(B)) {
      SndPlayMOD(MOD_BIT_Introtune);
    }
  }

  return 0;
}
