#ifndef DMA_H
#define DMA_H

#include "core/typedefs.h"

// bits 0-4 unused
//--- Destination Address Controls ---//
#define INCREMENT_DESTINATION (0 << 5) // bit 5 and 6 are 0
#define DECREMENT_DESTINATION (1 << 5)
#define FIXED_DESTINATION (1 << 6) 
#define RELOAD (1 << 5 | 1 << 6) 

//--- Source Address Controls ---//
#define INCREMENT_SOURCE (0 << 7)
#define DECREMENT_SOURCE (1 << 7)
#define FIXED_SOURCE (1 << 8)

//--- DMA Repeat ---//
#define DMA_REPEAT (1 << 9)

//--- DMA Transfer Type ---//
#define WORD (1 << 10) //defaults to halfword

//todo: add game pak

//--- DMA Start Timing ---//
#define IMMEDIATE (0 << 12)
#define WITH_VBLANK (1 << 12)
#define WITH_HBLANK (1 << 13)
#define SPECIAL_MODE (1 << 12 | 1 << 13)
// SPECIAL: DMA0=Prohibited, DMA1/DMA2=Sound FIFO, DMA3=Video Capture

//--- DMA Interrupt ---//
#define DMA_INTERRUPT_ENABLE (1 << 14)

// Pointer to dma as array
#define DMA ((DMA_DATA *)0x040000B0)

#define DMA_MEMCPY16 (DMA_ENABLE)
#define DMA_MEMCPY32 (DMA_WORD | DMA_ENABLE)
#define DMA_MEMSET16 (DMA_SRC_FIXED | DMA_ENABLE)
#define DMA_MEMSET32 (DMA_SRC_FIXED | DMA_WORD | DMA_ENABLE)


typedef struct __attribute__((packed)) {
  volatile u32 source;
  volatile u32 destination;
  volatile u16 wordCount;
  volatile u16 control;
} DMA_DATA;


inline void Dma3(void *dest, const void *src, u16 count, u16 flags) {
  DMA[3].source = (u32)src;
  DMA[3].destination = (u32)dest;
  DMA[3].wordCount = count;
  DMA[3].control = flags;
}

#endif // DMA_H
