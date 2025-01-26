#include "core/interrupts.h"

void vblankISR() {
  deltaTime += 1092; // 60 fps fixed constant value for seconds/frame
  REG_IF = 1;
}

extern void initializeVBI() {

  ISR = vblankISR;   // tell the GBA where my isr is
  DISPSTAT = 1 << 3; // tell display to fire vblank interrupts
  REG_IE |= 1;       // tell GBA to accept vblank interrupts
  REG_IME |= 1;      // tell GBA to enable intterupts
}
