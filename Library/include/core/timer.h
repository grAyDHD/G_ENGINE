#ifndef TIMER_H
#define TIMER_H
#include "core/typedefs.h"

//-------------------------------------//
//           Timer Flags               //
//-------------------------------------//

//--- Timer Prescaler (Bits 0–1) ---//
#define TMR_FREQ_1        (0 << 0) // No division (16.78MHz)
#define TMR_FREQ_64       (1 << 0)
#define TMR_FREQ_256      (2 << 0)
#define TMR_FREQ_1024     (3 << 0)

//--- Timer Cascade Mode (Bit 2) ---//
#define TMR_CASCADE       (1 << 2) // Timer N ticks when Timer N-1 overflows

//--- Timer Interrupt Enable (Bit 6) ---//
#define TMR_IRQ_ENABLE    (1 << 6) // Fire IRQ on overflow

//--- Timer Enable (Bit 7) ---//
#define TMR_ENABLE        (1 << 7) // Start timer

// Optional grouping
#define TMR_STOP          (0 << 7)

// 0000 0000 1100 0011
// 10=A 11=B 12=C 13=D 14=E 15=F
//-------------------------------------//
//           Timer Struct              //
//-------------------------------------//

// todo: move somewhere else
#define ALIGN4 __attribute__((aligned(4)))

typedef struct {
  volatile u16 value;    // TMxCNT_L: Reload value (written) / Counter (read)
  volatile u16 control;   // TMxCNT_H: Timer control flags
} ALIGN4 TMR_DATA;

// Pointer to timer array
#define TIMER ((TMR_DATA *)0x04000100)

void simpleWait(int delay);

//-------------------------------------//
//       Utility Function Ideas        //
//-------------------------------------//

// Set up and start a timer with given reload and flags
void timerStart(int timerID, u16 reloadValue, u16 flags);

// Stop a timer (clear enable bit)
void timerStop(int timerID);

// Wait for a simple duration (blocking/delay)
void simpleWait(int delay);

// Return current count value (live read)
u16 timerRead(int timerID);


#endif
