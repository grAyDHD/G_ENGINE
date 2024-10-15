#include "typedefs.h"
#include <dmg.h>
#include <draw.h>
#include <gfx.h>

#define TMR0 *(volatile u16 *)(0x04000100)
#define TMR0_D *(volatile u16 *)(0x04000102)
#define TMRSTRT 0x80

#define PLAYNOW 0x8000 // 1000 0000 0000 0000

// typedef void (*fnptr)(void);

// #define ISR_VECTOR *(fnptr *)(0x03007FFC)     // interrupt vector &
#define REG_IME (*(volatile u16 *)0x04000208) // master enable, set bit 0 to 1
#define REG_IE (*(volatile u16 *)0x04000200)  // enable specific interrupts
#define REG_IF (*(volatile u16 *)0x04000202)  // check/acknowledge interrupts
#define IRQTM0 0x0008

int notes[2] = {0x2C, 0x416};
int currentNote = 0;

void ISR() {

  if (REG_IF & IRQTM0) { // Check if Timer 0 interrupt occurred
    FREQ_1 = (notes[currentNote] | PLAYNOW);
    currentNote++;
    if (currentNote > 1) {
      currentNote = 0;
    }
    REG_IF = IRQTM0; // Acknowledge Timer 0 interrupt
  }
}
// code that ISR_VECTOR MUST BE ARM, not -mthumb
// create a switchboard:
// need _isr_table[]
// IRQ_REC struct
// irq_init() irq_set_master()
// set master isr, init initializes interrupt table and interrupts themselves
// irq_enable() irq_disable
// functions to enable and disable interrupts. These will take care of both
// REG_IE and whatever reg the sender bit is on Keep these bits in an internal
// table called _irq_senders[] and to use these, the input parameter of these
// functions need to be the index of the interrupt, not the interrupt flag.
// Hence II_foo counterparts for IRQ_foo flags irq_set()/irq_add()/irq_delete()
// irq set allows full prioritization of isr's
// add will replace currens isr for a given interrupt, or add one at end of the
// list delete will delete on and correct the list for empty space
//
void foo() {
  //  ISR_VECTOR = ISR;
  // enable/start timer
  // REG_IE |= enable matching bit
  REG_IME = 1; // enable interrupts
}
int main() {
  ENABLE_SOUND();
  DMG_STEREO_OUTPUT = 0xFF77;
  ENV_1 = 0xF393;

  //  ISR_VECTOR = ISR;
  REG_IME = 0x1;  // enable global interrupts;
  REG_IE |= 0x08; // enable tm0 interrupt

  TMR0 = 0x4003;   //
  TMR0_D = 0x00C3; // timer started, irq enabled, prescaler 1024

  while (1) {
  }
  return 0;
}

// reg = 2048 - ((4194304/hz)/32)
// reg = 2048 - ((4194304/352)/32)
// 65hz or 44, 0x2C
// 130hz or 1046, 0x416
//
// 0000 0000 0010 1100
// 0000 0100 0001 0110
// timer 0 controls speed of logo
// timer 1 handles color change of logo
// timer 2 interrupts for dmg1 melody
// timer 3 spfx, melody speed?
/*
void ISR(void) {             // ISR
  __asm__ volatile("bx pc\n" // Switch to ARM mode
                   ".arm\n"  // ARM mode instructions start here
  );

  if (REG_IF & IRQTM0) { // check REG_IF for 0x08- tm0 interrupt
    FREQ = (notes[currentNote] | PLAYNOW);
    currentNote++;
    if (currentNote > 1) {
      currentNote = 0;
    }
    REG_IF = IRQTM0; // acknowledge interrupt
  }

  __asm__ volatile("bx pc\n"  // Switch back to THUMB mode
                   ".thumb\n" // THUMB mode instructions start here
  );
}
*/

// void __attribute__((target("arm"), interrupt("IRQ"))) ISR(void) {

// a timer is like an hour glass, 0-3.
// each hourglass is 16 bits, 0-65535
// these measure time based on clock speed:
// 16.78 MHz
// to set up a tmer (_L registers):
// writing to _L is like deciding how many grains of sand
// / ticks to put in hourglass initially,
// but, this doesn't start the timer-
// just the 'reload value'
//
// if you read from _L regs while timer running-
// you see how much sand is left before hourglass
// 'flips over', or overflows
//
// to start the timer:
// _H registers- the handles you flip to start hourglass
// once you flip the handle(set start bit to 1)
// sand starts falling
// when timer runs out of sand(overflows),
// it automatically refills to initially set level.
// starts over until you stop it.
// _H bits:
// 0-1 Prescaller 0=F/1, 1=F/64, 2=F/256, 3=F/1024
// 2 count-up timing (0=normal, 1:
// count up enabled: prescaler val ignored, time is incremented each time when
// th previous counter overflows doesn't work on timer 0) F= System
// Clock(16.78MHz) 3-5 not used 6 IRQ enable (0=disable, 1=IRQ on timer
// overflow) 7 Timer Start/Stop (0=Stop, 1=Start) 8-15 not used. Timer tick rate
// = clock frequency/prescaler value: for presaler val of 1024- trate =
// 16,780,000/1024 ~= 16,406 ticks per second. bpm: timer ticks = 60 seconds/bpm
// * cpu/prescaler beat interval = 60sec/bpm- 60/120 = .5sec per beat 60 bpm,
// 1024, 16387; 120 = 8193 90 = 256, 37455
