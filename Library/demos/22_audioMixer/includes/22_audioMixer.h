#ifndef AUDIOMIXER_H
#define AUDIOMIXER_H
#include "guitarData.h"
#include "graphics/m3Text.h"
#include "core/typedefs.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"
#include "core/timer.h"
#include "input/in.h"
#include "core/dma.h"
// todo: split audio headers up, dmg, direct sound, and possibly a more general file
#include "audio/dmg.h"


// Direct Sound control and Sound 1-4 output ratio
#define SOUNDCNT_H *(volatile u16 *)0x04000082

#define REG_SGFIFOA *(volatile u32 *)0x40000A0
#define REG_SGFIFOA_L *(volatile u16 *)0x40000A0
#define REG_SGFIFOA_H *(volatile u16 *)0x40000A2

#define ISR *(fnptr *)0x03007FFC   // put function address
#define DISPSTAT *(u16 *)0x4000004 // bit three vblank timer
#define REG_IME *(u16 *)0x4000208  // set bit 1 to enable interrupts
#define REG_IE *(u16 *)0x4000200   // first bit for vblank, |= 1
#define REG_IF *(u16 *)0x4000202   // set first bit to acknowledge interrupt |= 1

// todo: set up VSync, looks straightforward
// functions in Math.s of deku tutorial

typedef struct _SOUND_CHANNEL {
  s8 *data;
  u32 pos;
  u32 inc;
  u32 vol;
  u32 length;
  u32 loopLength;

} SOUND_CHANNEL;

typedef struct _SOUND_VARS {
  s8 *mixBufferBase; //base address of overall buffers, both are back to back starting here
  s8 *curMixBuffer;
  u32 mixBufferSize; // 
  u8 activeBuffer;

} SOUND_VARS;

s8 sndMixBuffer[304 * 2]; //IN_EWRAM;

SOUND_CHANNEL sndChannel[2];
SOUND_VARS sndVars;

void soundInit() {
//timer length, frequency, buffer size
//64612, 18157, 304
  s32 i;

  // todo: wtf is this lol
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

  TIMER[0].count = 64612; //vblank length
  TIMER[0].control = TM_ENABLE; // 0x0080 

  DMA[1].wordCount = 0;
  DMA[1].control = 0;
  DMA[1].destination = (u32)&REG_SGFIFOA;
}

void IrqNull() {}

void vblankAudioISR(){ 
  REG_IF = 1;

  if (sndVars.activeBuffer == 1) // buffer 1 just got over
  {
    // Start playing buffer 0
    DMA[1].control = 0;
    DMA[1].source = (u32)sndVars.mixBufferBase;
    DMA[1].control =
        DMA_DEST_FIXED | DMA_REPEAT | DMA_WORD | DMA_MODE_FIFO | DMA_ENABLE;

    // Set the current buffer pointer to the start of buffer 1
    sndVars.curMixBuffer = sndVars.mixBufferBase + sndVars.mixBufferSize;
    sndVars.activeBuffer = 0;
  } else // buffer 0 just got over
  {

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
  s16 tempBuffer[304];

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


#endif // AUDIOMIXER_H
