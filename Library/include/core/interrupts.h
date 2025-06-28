#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "core/typedefs.h"

// todo: move to video header
#define DISPSTAT *(u16 *)0x4000004
// set bit 1 to enable interrupts

#define ISR *(fnptr *)0x03007FFC // put function address
// bit three vblank timer

// todo: phase out IME and IE syntax with more readable IRQ master/enabler
// only two options: 0 or 1, disable/enable
#define IRQ_MASTER *(u16 *)0x4000208
#define REG_IME *(u16 *)0x4000208

#define IRQ_ENABLER *(u16 *)0x4000200
#define REG_IE *(u16 *)0x4000200
#define IRQ_VBLANK (1 << 0)
#define IRQ_HBLANK (1 << 1)
#define IRQ_VCOUNTER (1 << 2) // what is this exactly? v counter match
#define IRQ_TMR0 (1 << 3)
#define IRQ_TMR1 (1 << 4)
#define IRQ_TMR2 (1 << 5)
#define IRQ_TMR3 (1 << 6)
#define IRQ_SERIAL (1 << 7)
#define IRQ_DMA0 (1 << 8)
#define IRQ_DMA1 (1 << 9)
#define IRQ_DMA2 (1 << 10)
#define IRQ_DMA3 (1 << 11)
#define IRQ_KEYPAD (1 << 12)
#define IRQ_GAMEPAK (1 << 13)

// set first bit to acknowledge interrupt |= 1
#define IRQ_ACKNOWLEDGER *(u16 *)0x4000202


inline void irqMaster(u16 state) {
  IRQ_MASTER = state;
};

inline void irqEnable(u16 irqMask) {
  IRQ_ENABLER |= irqMask;  
};

inline void irqDisable(u16 irqMask) {
  IRQ_ENABLER &= ~irqMask;
};

inline void irqAcknowledge(u16 irqMask) {
  IRQ_ACKNOWLEDGER = irqMask;
};
//int irqEnabled(u16 irqMask); //check if specific interrupt is enabled

// vblank interrupt handler, for counting frames to calculate delta time
extern volatile s32 deltaTime;

void vblankISR();

void initializeVBI();
#endif

