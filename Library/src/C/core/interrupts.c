#include "core/interrupts.h"

// todo: give better name showing delta time
void vblankISR() {
  // todo: create constant so 1092 is more clear
  deltaTime += 1092; // 60 fps 16.16 fixed point constant value for seconds/frame
  // 1092 represents 1/60th of a second (one VBLANK cycle)
  // todo: I have no clue what the fuck this is doing
  // REG_IF = 1;
  // todo complete, I now know what the fuck this is doing 
  irqAcknowledge(IRQ_VBLANK);
}


// todo: reconsider approach, look into switch table
extern void initializeVBI() {
  ISR = vblankISR;   // tell the GBA where my isr is
  // todo: update display/video header
  DISPSTAT = 1 << 3; // tell display to fire vblank interrupts
//  REG_IE |= 1;       // tell GBA to accept vblank interrupts
  irqEnable(IRQ_VBLANK);
 // REG_IME |= 1;      // tell GBA to enable intterupts
  // todo: should this happen elsewhere, or will this be a broad initializer
  irqMaster(ON);
}
