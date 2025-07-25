#ifndef DMA_H
#define DMA_H

#include "core/typedefs.h"

#define ALIGN4 __attribute__((aligned(4)))

//-------------------------------------//
//           DMA Control Flags         //
//-------------------------------------//

//--- Destination Address Control (Bits 5-6) ---//
#define DMA_DEST_INCREMENT (0 << 5)
#define DMA_DEST_DECREMENT (1 << 5)
#define DMA_DEST_FIXED (2 << 5)
#define DMA_DEST_RESET (3 << 5) // Used with repeat (only for DMA0/1/2)

//--- Source Address Control (Bits 7-8) ---//
#define DMA_SRC_INCREMENT (0 << 7)
#define DMA_SRC_DECREMENT (1 << 7)
#define DMA_SRC_FIXED (2 << 7)

//--- DMA Repeat Mode (Bit 9) ---//
#define DMA_REPEAT (1 << 9)

//--- Transfer Size (Bit 10) ---//
#define DMA_16BIT (0 << 10) // Default
#define DMA_32BIT (1 << 10)

//--- DMA Timing / Start Type (Bits 12–13) ---//
#define DMA_START_NOW (0 << 12)
#define DMA_START_VBLANK (1 << 12)
#define DMA_START_HBLANK (2 << 12)
#define DMA_START_SPECIAL (3 << 12) // Sound FIFO (DMA1/2), Video Capture (DMA3)

//--- IRQ on Completion (Bit 14) ---//
#define DMA_IRQ_ENABLE (1 << 14)

//--- DMA Enable (Bit 15) ---//
#define DMA_ENABLE (1 << 15)

//-------------------------------------//
//        DMA Composite Modes          //
//-------------------------------------//

#define DMA_MEMCPY16 (DMA_ENABLE)
#define DMA_MEMCPY32 (DMA_ENABLE | DMA_32BIT)
#define DMA_MEMSET16 (DMA_ENABLE | DMA_SRC_FIXED)
#define DMA_MEMSET32 (DMA_ENABLE | DMA_SRC_FIXED | DMA_32BIT)

//-------------------------------------//
//           DMA Struct                //
//-------------------------------------//
typedef struct {
  volatile u32 source;
  volatile u32 destination;
  volatile u16 wordCount;
  volatile u16 control;
} ALIGN4 DMA_DATA;

// Pointer to DMA channels as array
#define DMA ((DMA_DATA *)0x040000B0)

//-------------------------------------//
//         DMA Utility Function        //
//-------------------------------------//

inline void Dma0(void *dest, const void *src, u16 count, u16 flags) {
  DMA[0].source = (u32)src;
  DMA[0].destination = (u32)dest;
  DMA[0].wordCount = count;
  DMA[0].control = flags;
}

inline void Dma2(void *dest, const void *src, u16 count, u16 flags) {
  DMA[2].source = (u32)src;
  DMA[2].destination = (u32)dest;
  DMA[2].wordCount = count;
  DMA[2].control = flags;
}

inline void Dma3(void *dest, const void *src, u16 count, u16 flags) {
  DMA[3].source = (u32)src;
  DMA[3].destination = (u32)dest;
  DMA[3].wordCount = count;
  DMA[3].control = flags;
}

static inline void dmaStart(int channel, void *dst, const void *src, u16 count,
                            u16 flags) {
  DMA[channel].source = (u32)src;
  DMA[channel].destination = (u32)dst;
  DMA[channel].wordCount = count;
  DMA[channel].control = flags;
}

static inline void dmaStop(int channel) {
  DMA[channel].control = 0; // Clear all flags to disable DMA
}

#endif // DMA_H
