#include "draw.h"
#include "engine.h"
#include "gfx.h"
#include "typedefs.h"
#define TIMER_DATA(timer) *(volatile u16 *)(0x04000100 + (timer << 2))
#define TIMER_CONTROL(timer) *(volatile u16 *)(0x04000102 + (timer << 2))

/*
 * 4 timers, numbered 0-3
 * Each timer consists of two consecutive 16-bit registers.
 * Timers begin at 0x04000100, offset of 2 for control register:
 * Timer 0: 0x0400100, Timer 0 Control: 0x04000102
 * Timer 1: 0x0400104, Timer 0 Control: 0x04000104
 * Timer 2: 0x0400108, Timer 2 Control: 0x0400010A
 * Timer 3: 0x040010C, Timer 3 Control: 0x0400010E
 *
 * Note: hardware timers generally reserve for timed dma transfers,
 * like in sound mixers, not game timers.  Can also use to time functions.
 *
 * TIMER x Registers;         0x04000100 + 04h*x
 *  set TIMER_x = n.  n is timer starting value.
 *  when enabled, timer will reset to n.
 *  doesn't set timer value, but rather initial value for next cycle
 *  set n to negative value for short timers, will overflow when it reaches 0
 *
 * TIMER_CONTROL x Registers: 0x04000102 + 04h*x
 * bit 0-1 Fr, Timer Frequency(y) y=number of cycles, 1, 64, 256, or 1024, 0-3
 * meaning, each increment of timer jumps by this value.
 * bits respectively
 * bit 2 CM Cascade mode, when set, only increments when prev timer overflows
 * don't use on timer 0
 * bit 6 TM_IRQ raise an interrupt on overflow
 * bit 7 TM_ENABLE enable this timer
 *
 * Chat GPT calculated the following, will manually verify
 *
 * Max register value of 65,535
 * Prescalers (data register bits 0-3)
 * TM_FREQ_1  3.906 ms per overflow
 * TM_FREQ_64 240 ms per overflow
 * TM_FREQ_256  1 second per overflow
 * TM_FREQ_1024 4 seconds per overflow
 *
 * 1  	16.78 MHz	  59.59 ns
 * 64 	262.21 kHz	3.815 μs
 * 256 	65.536 kHz	15.26 μs
 * 1024	16.384 kHz	61.04 μ
 *
 * Cascade on/off
 * Timer enable/disable
 */

// Timer control bitfields
#define TM_FREQ_1 0x0000     // No prescaling
#define TM_FREQ_64 0x0001    // Timer frequency 64 cycles
#define TM_FREQ_256 0x0002   // Timer frequency 256 cycles
#define TM_FREQ_1024 0x0003  // Timer frequency 1024 cycles
#define TM_CASCADE 0x0004    // Cascade mode
#define TM_IRQ_ENABLE 0x0040 // Enable timer overflow interrupt
#define TM_ENABLE 0x0080     // Enable the timer

// #define ALIGN(n)    __attribute__((aligned(n)))
// ensures data aligned to 4 byte/32 bit boundary
#define ALIGN4 __attribute((aligned(4)))

// Timer struct definition
typedef struct {
  volatile u16 count;   // Timer data register (TIMER_x)
  volatile u16 control; // Timer control register (TIMER_CONTROL_x)
} ALIGN4 TMR;

// Pointer to timer array
#define TIMER ((TMR *)0x04000100)

//
int main() {
  DSPC = MODE3 | BG2;
  Coordinate rectXY = {0, 0};

  // Overflow every ~1 second:
  // 0x4000 ticks @ FREQ_1024
  TIMER[0].count = -0x4000;
  TIMER[0].control = TM_ENABLE | TM_FREQ_1024;

  TIMER[1].count = 0;
  TIMER[1].control = TM_ENABLE | TM_CASCADE;

  int time = TIMER[1].count;

  while (1) {
    // if check will pass one time whenever timer 1 increments
    if (time != TIMER[1].count) {
      time = TIMER[1].count;

      // clear rectangle
      drawRect(rectXY, 10, 10, RGB(0, 0, 0));

      // update rectangle position
      rectXY.x += 10;

      // re draw rectangle at new postion
      drawRect(rectXY, 10, 10, RGB(10, 1, 17));
    }
  }

  return 0;
}

// toggle individual bits with XOR:
//  TIMER[0].control ^= TM_ENABLE
//  to pause timer without resetting, keep timer enabled
//  as when timer disabled then enabled, it resets to count value
//  but TIMER[0].control ^= TM_CASCADE will enable cascade mode-
//  not resetting the timer, and stopping incrementing as no timer is
//  cascading into timer 0
//
//    drawRect(Coordinate start, int width, int height, unsigned short
//    color)
