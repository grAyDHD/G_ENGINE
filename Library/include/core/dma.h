#ifndef DMA_H
#define DMA_H

#include "core/typedefs.h"

// Pointer to dma as array
#define DMA ((DMA_DATA *)0x040000B0)

//--- Destination Address Controls ---//
#define INCREMENT_DESTINATION (0 << 5) 
#define DECREMENT_DESTINATION (1 << 5)
#define FIXED_DESTINATION     (2 << 5) 
#define RELOAD                (3 << 5) 

//--- Source Address Controls ---//
#define INCREMENT_SOURCE      (0 << 7)
#define DECREMENT_SOURCE      (1 << 7)
#define FIXED_SOURCE          (2 << 7)

//--- DMA Repeat ---//
#define REPEAT                (1 << 9)

//--- DMA Transfer Type ---//
#define HALFWORD              (0 << 10)
#define WORD                  (1 << 10)

//todo: add game pak

//--- DMA Start Timing ---//
#define IMMEDIATE             (0 << 12) 
#define WITH_VBLANK           (1 << 12)
#define WITH_HBLANK           (2 << 12)
#define SPECIAL_MODE          (3 << 12)
// SPECIAL_MODE: 
// DMA0=Prohibited, DMA1/DMA2=Sound FIFO, DMA3=Video Capture

//--- DMA Interrupt ---//
#define DMA_INTERRUPT_ENABLE (1 << 14)

//--- DMA ENABLE ---//
#define ENABLE (1 << 15)

#define DMA_MEMCPY16 (ENABLE)
#define DMA_MEMCPY32 (WORD | ENABLE)
#define DMA_MEMSET16 (FIXED_SOURCE | ENABLE)
#define DMA_MEMSET32 (FIXED_SOURCE | WORD | ENABLE)


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
