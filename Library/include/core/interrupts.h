#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "core/typedefs.h"

#define ISR *(fnptr *)0x03007FFC // put function address
// bit three vblank timer
#define DISPSTAT *(u16 *)0x4000004
// set bit 1 to enable interrupts
#define REG_IME *(u16 *)0x4000208
// first bit for vblank, |= 1
#define REG_IE *(u16 *)0x4000200
// set first bit to acknowledge interrupt |= 1
#define REG_IF *(u16 *)0x4000202

// vblank interrupt handler, for counting frames to calculate delta time
extern volatile s32 deltaTime;
void vblankISR() {
  deltaTime += 1092; // 60 fps fixed constant value for seconds/frame
  REG_IF = 1;
}

#endif
