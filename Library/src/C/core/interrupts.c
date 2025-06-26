#include "core/interrupts.h"

// todo: give better name showing delta time
void vblankISR() {
  // todo: create constant so 1092 is more clear
  deltaTime += 1092; // 60 fps fixed constant value for seconds/frame
  // todo: I have no clue what the fuck this is doing
  REG_IF = 1;//IF interrupt flag, acknowledge
}


// todo: reconsider approach, look into switch table
extern void initializeVBI() {
  ISR = vblankISR;   // tell the GBA where my isr is
  DISPSTAT = 1 << 3; // tell display to fire vblank interrupts
  REG_IE |= 1;       // tell GBA to accept vblank interrupts
  REG_IME |= 1;      // tell GBA to enable intterupts
}
