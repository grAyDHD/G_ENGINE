#include "../includes/23_mPlayer.h"
#include "audio/mixer.h"
#include "core/interrupts.h"

void irqDispatch() {
  // check IRQ_FLAGS
  int flags = IRQ_FLAGS;
  if (flags & IRQ_DMA1) {
    dmaAudioIsr();
    irqAcknowledge(IRQ_DMA1);
  }

  if (flags & IRQ_VBLANK) {
    vblankISR();
    irqAcknowledge(IRQ_VBLANK);
  }
}
