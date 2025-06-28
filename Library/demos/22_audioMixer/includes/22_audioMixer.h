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
#include "core/interrupts.h"


// todo: refactor defines into lib headers.  WIP
#define SOUNDCNT_H *(volatile u16 *)0x04000082

#define REG_SGFIFOA *(volatile u32 *)0x40000A0
#define REG_SGFIFOA_L *(volatile u16 *)0x40000A0
#define REG_SGFIFOA_H *(volatile u16 *)0x40000A2

#define DISPSTAT *(u16 *)0x4000004 // bit three vblank timer
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
  // I think it enables/sets direct sound channels 
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

  // todo: make clear why using timer, as special mode is set elsewhere
  TIMER[0].value = 64612; //vblank length
  TIMER[0].control |= TIMER_ENABLE; // |= to not overwrite interrupt enable flag

  DMA[1].wordCount = 0;
  DMA[1].control = 0;
  DMA[1].destination = (u32)&REG_SGFIFOA;
}

void IrqNull() {}
void vblankAudioISR(){ 
  irqAcknowledge(IRQ_VBLANK);

  if (sndVars.activeBuffer == 1) // buffer 1 just got over
  {
    // Start playing buffer 0
    DMA[1].control = 0;
    DMA[1].source = (u32)sndVars.mixBufferBase;
    DMA[1].control = DMA_DEST_FIXED | DMA_SRC_INCREMENT | DMA_REPEAT | DMA_32BIT | DMA_START_SPECIAL | DMA_ENABLE;

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

void timerISR() {


  irqAcknowledge(IRQ_TMR1);
  if (sndVars.activeBuffer == 1) // buffer 1 just got over
  {
    // Start playing buffer 0
    DMA[1].control = 0;
    DMA[1].source = (u32)sndVars.mixBufferBase;
    DMA[1].control = DMA_DEST_FIXED | DMA_SRC_INCREMENT | DMA_REPEAT | DMA_32BIT | DMA_START_SPECIAL | DMA_ENABLE;

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

void initializeTMRI() {
  // todo: improve approach by creating an interrupt table in c I can then further optimize in asm
  ISR = timerISR;
  // have timer fire interrupts
  // have timer fire interrupts, have gba accept timer interrups (REG_IE)
  TIMER[1].value = 65257;
  TIMER[1].control |= TIMER_ENABLE | TIMER_IRQ_ENABLE;

//  REG_IE |= IRQ_TMR1;
  irqEnable(IRQ_TMR1);
}

void initializeVBI() {
  ISR = vblankAudioISR;   // tell the GBA where my isr is
  // todo: update display header
  DISPSTAT = 1 << 3; // tell display to fire vblank interrupts
//  REG_IE |= 1;       // tell GBA to accept vblank interrupts
  irqEnable(IRQ_VBLANK);
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
