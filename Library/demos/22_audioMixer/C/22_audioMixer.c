#include "../includes/22_audioMixer.h"
#include "core/typedefs.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"
#include "core/timer.h"
#include "input/in.h"


s8 sndMixBuffer[304 * 2]; //IN_EWRAM;

// Direct Sound control and Sound 1-4 output ratio
#define SOUNDCNT_H *(volatile u16 *)0x04000082
#define MASTER_SOUND_SYSTEM *(volatile u16 *)0x04000084
#define ENABLE_SOUND() (MASTER_SOUND_SYSTEM |= 0x0080)

#define REG_SGFIFOA *(volatile u32 *)0x40000A0
#define REG_SGFIFOA_L *(volatile u16 *)0x40000A0
#define REG_SGFIFOA_H *(volatile u16 *)0x40000A2

#define REG_DM1SAD *(volatile u32 *)0x40000BC
#define REG_DM1SAD_L *(volatile u16 *)0x40000BC
#define REG_DM1SAD_H *(volatile u16 *)0x40000BE

#define REG_DM1DAD *(volatile u32 *)0x40000C0
#define REG_DM1DAD_L *(volatile u16 *)0x40000C0
#define REG_DM1DAD_H *(volatile u16 *)0x40000C2

#define REG_DM1CNT *(volatile u32 *)0x40000C4
#define REG_DM1CNT_L *(volatile u16 *)0x40000C4
#define REG_DM1CNT_H *(volatile u16 *)0x40000C6

#define BIT00 0x1
#define BIT01 0x2
#define BIT02 0x4
#define BIT03 0x8
#define BIT04 0x10
#define BIT05 0x20
#define BIT06 0x40
#define BIT07 0x80
#define BIT08 0x100
#define BIT09 0x200
#define BIT10 0x400
#define BIT11 0x800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000
#define BIT16 0x10000
#define BIT17 0x20000
#define BIT18 0x40000
#define BIT19 0x80000
#define BIT20 0x100000
#define BIT21 0x200000
#define BIT22 0x400000
#define BIT23 0x800000
#define BIT24 0x1000000
#define BIT25 0x2000000
#define BIT26 0x4000000
#define BIT27 0x8000000
#define BIT28 0x10000000
#define BIT29 0x20000000
#define BIT30 0x40000000
#define BIT31 0x80000000

#define REG_DM3SAD *(volatile u32 *)0x40000D4
#define REG_DM3SAD_L *(volatile u16 *)0x40000D4
#define REG_DM3SAD_H *(volatile u16 *)0x40000D6
#define REG_DM3DAD *(volatile u32 *)0x40000D8
#define REG_DM3DAD_L *(volatile u16 *)0x40000D8
#define REG_DM3DAD_H *(volatile u16 *)0x40000DA
#define REG_DM3CNT *(volatile u32 *)0x40000DC
#define REG_DM3CNT_L *(volatile u16 *)0x40000DC
#define REG_DM3CNT_H *(volatile u16 *)0x40000DE

// functions in Misc.c
inline void Dma3(void *dest, const void *src, u32 count, u16 flags) {
  REG_DM3SAD = (u32)src;
  REG_DM3DAD = (u32)dest;
  REG_DM3CNT_L = count;
  REG_DM3CNT_H = flags;
}

#define DMA_ENABLE BIT15
#define DMA_WORD BIT10
#define DMA_SRC_FIXED BIT08
#define PAL_BG ((u16 *)0x5000000)
#define DMA_MEMCPY16 (DMA_ENABLE)
#define DMA_MEMCPY32 (DMA_WORD | DMA_ENABLE)
#define DMA_MEMSET16 (DMA_SRC_FIXED | DMA_ENABLE)
#define DMA_MEMSET32 (DMA_SRC_FIXED | DMA_WORD | DMA_ENABLE)

inline void LoadBgPal16(u8 idx, const u16 *src) {
  Dma3((void *)(((void *)PAL_BG) + (idx << 5)), (void *)src, 16, DMA_MEMCPY16);
}

extern u8 *htoa(u32 number);

// functions in Math.s
extern void VSync();
//SOUND_CHANNEL sndChannel[SND_MAX_CHANNELS];
//SOUND_VARS sndVars;

/*
 All registers are defined and accessible
 SoundInit() function exists and runs without crashing
 Timer 0 is running at the correct frequency
 DMA is configured for audio transfer
 Master sound is enabled
 No compile errors, no runtime crashes

Essential Tasks:

Enable master sound (REG_SOUNDCNT_X)
Configure Direct Sound A (REG_SOUNDCNT_H)
Set up Timer 0 for sample rate timing
Configure DMA1 to feed the audio FIFO
*/
typedef struct _SOUND_CHANNEL {
  s8 *data;
  u32 pos;
  u32 inc;
  u32 vol;
  u32 length;
  u32 loopLength;

} SOUND_CHANNEL;

typedef struct _SOUND_VARS {
  s8 *mixBufferBase;
  s8 *curMixBuffer;
  u32 mixBufferSize;
  u8 activeBuffer;

} SOUND_VARS;

SOUND_CHANNEL sndChannel[2];
SOUND_VARS sndVars;


void soundInit() {
//timer length, frequency, buffer size
//65612, 18157, 304
  s32 i;

  SOUNDCNT_H = 0x0B04;
  ENABLE_SOUND();

  sndVars.mixBufferSize = 304;
  sndVars.mixBufferBase = sndMixBuffer;
  sndVars.curMixBuffer = sndVars.mixBufferBase;
  sndVars.activeBuffer = 1;

  // initialize channel structures
  for (i = 0; i < 2; i++) { // 2 is cur max channels
    sndChannel[i].data = 0;
    sndChannel[i].pos = 0;
    sndChannel[i].inc = 0;
    sndChannel[i].vol = 0;
    sndChannel[i].length = 0;
    sndChannel[i].loopLength = 0;
  }


  TIMER[0].count = 64612; //vblank length???
  TIMER[0].control = TM_ENABLE; // 0x0080 

  REG_DM1CNT = 0; //clear DMA 1 register?
  REG_DM1DAD = (u32)&REG_SGFIFOA; // destination address, FIFO register
}

void IrqNull() {}

#define DMA_DEST_FIXED BIT06
#define DMA_REPEAT BIT09
#define DMA_WORD BIT10
#define DMA_MODE_FIFO (BIT12 | BIT13)
#define DMA_ENABLE BIT15

#define ISR *(fnptr *)0x03007FFC // put function address
// bit three vblank timer
#define DISPSTAT *(u16 *)0x4000004
// set bit 1 to enable interrupts
#define REG_IME *(u16 *)0x4000208
// first bit for vblank, |= 1
#define REG_IE *(u16 *)0x4000200
// set first bit to acknowledge interrupt |= 1
#define REG_IF *(u16 *)0x4000202

volatile int frameReady = 0;

void vblankAudioISR(){
  REG_IF = 1;

  if (sndVars.activeBuffer == 1) // buffer 1 just got over
  {
    // Start playing buffer 0
    REG_DM1CNT = 0;
    REG_DM1SAD = (u32)sndVars.mixBufferBase;
    REG_DM1CNT_H =
        DMA_DEST_FIXED | DMA_REPEAT | DMA_WORD | DMA_MODE_FIFO | DMA_ENABLE;

    // Set the current buffer pointer to the start of buffer 1
    sndVars.curMixBuffer = sndVars.mixBufferBase + sndVars.mixBufferSize;
    sndVars.activeBuffer = 0;
  } else // buffer 0 just got over
  {
    // DMA points to buffer 1 already, so don't bother stopping and resetting it

    // Set the current buffer pointer to the start of buffer 0
    sndVars.curMixBuffer = sndVars.mixBufferBase;
    sndVars.activeBuffer = 1;
  }
}

void initializeVBI() {
  ISR = vblankAudioISR;   // tell the GBA where my isr is
  DISPSTAT = 1 << 3; // tell display to fire vblank interrupts
  REG_IE |= 1;       // tell GBA to accept vblank interrupts
}

void SndMix() {
  s32 i, curChn;

  // If you want to use a higher frequency than 18157,
  // you'll need to make this bigger.
  // To be safe, it would be best to set it to the buffer
  // size of the highest frequency we allow in freqTable
  s16 tempBuffer[304];

  // zero the buffer
  i = 0;
  Dma3(tempBuffer, &i, sndVars.mixBufferSize * sizeof(s16) / 4, DMA_MEMSET32);

  for (curChn = 0; curChn < 2; curChn++) {
    SOUND_CHANNEL *chnPtr = &sndChannel[curChn];

    // check special active flag value
    if (chnPtr->data != 0) {
      // this channel is active, so mix its data into the intermediate buffer
      for (i = 0; i < sndVars.mixBufferSize; i++) {
        // mix a sample into the intermediate buffer
        tempBuffer[i] += chnPtr->data[chnPtr->pos >> 12] * chnPtr->vol;
        chnPtr->pos += chnPtr->inc;

        // loop the sound if it hits the end
        if (chnPtr->pos >= chnPtr->length) {
          // check special loop on/off flag value
          if (chnPtr->loopLength == 0) {
            // disable the channel and break from the i loop
            chnPtr->data = 0;
            i = sndVars.mixBufferSize;
          } else {
            // loop back
            while (chnPtr->pos >= chnPtr->length) {
              chnPtr->pos -= chnPtr->loopLength;
            }
          }
        }
      } // end for i = 0 to bufSize
    }   // end data != 0
  }     // end channel loop

  // now downsample the 16-bit buffer and copy it into the actual playing buffer
  for (i = 0; i < sndVars.mixBufferSize; i++) {
    // >>6 to divide off the volume, >>2 to divide by 4 channels
    // to prevent overflow. Could make a define for this up with
    // SOUND_MAX_CHANNELS, but I'll hardcode it for now
    sndVars.curMixBuffer[i] = tempBuffer[i] >> 8;
  }

} // SndMix
int main() { 
  initializeVBI();
  soundInit();

  REG_IME |= 1;      // tell GBA to enable intterupts
  DSPC = MODE3 | BG2;

 
  while(1) {
    VBLANK();
      updateKeys();


    if (keyTapped(LT)) {
        
      sndChannel[0].data = 0; // clear channel
      sndChannel[0].pos = 0; //set to beginning?
      sndChannel[0].inc = 4096;
      sndChannel[0].vol = 64;
      sndChannel[0].length = 28419 << 12;
      sndChannel[0].loopLength = 0; // no loop
      sndChannel[0].data = (s8 *)guitarData;
    }

    SndMix();
  }

    return 0;
}
