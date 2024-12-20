#ifndef TIMER_H
#define TIMER_H
#include "core/typedefs.h"

#define TIMER_DATA(timer) *(volatile u16 *)(0x04000100 + (timer << 2))
#define TIMER_CONTROL(timer) *(volatile u16 *)(0x04000102 + (timer << 2))

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

void simpleWait(int delay);

#endif
