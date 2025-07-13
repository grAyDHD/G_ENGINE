#include "../includes/directSound.h"
#include "audio/audio.h"
#include "audio/dmg.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "core/timer.h"
#include "core/typedefs.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"
#include "input/in.h"

AudioChannel channel[4];

Mixbuffer mixbuf = {0};

void initMixChannels() {
  // Channel 0: Music loop (tharp)
  channel[0].data = tharp16k;
  channel[0].position = 0;
  channel[0].length = tharp16klen;
  channel[0].volume = 32;
  channel[0].isPlaying = 0;
  channel[0].looping = 1;
  channel[0].fadeOut = 0;
  channel[0].lastSample = 0;

  // Channel 1: Sample A (one-shot kick)
  channel[1].data = kick16k;
  channel[1].position = 0;
  channel[1].length = kick16klen;
  channel[1].volume = 32;
  channel[1].isPlaying = 0;
  channel[1].looping = 0;
  channel[1].fadeOut = 0;
  channel[1].lastSample = 0;

  // Channel 2: Sample B (one-shot snare)
  channel[2].data = snare16k;
  channel[2].position = 0;
  channel[2].length = snare16klen;
  channel[2].volume = 32;
  channel[2].isPlaying = 0;
  channel[2].looping = 0;
  channel[2].fadeOut = 0;
  channel[2].lastSample = 0;

  // Channel 3: Sample B (one-shot hi hat)
  channel[3].data = hat16k;
  channel[3].position = 0;
  channel[3].length = hat16klen;
  channel[3].volume = 32;
  channel[3].isPlaying = 0;
  channel[3].looping = 0;
  channel[3].fadeOut = 0;
  channel[3].lastSample = 0;
}

void mixAudio() {
  s32 i, ch;
  s16 tempBuffer[BUFFER_SIZE];
  s8 *targetBuffer = (mixbuf.activeBuffer == bufA) ? mixbuf.bufB : mixbuf.bufA;

  i = 0;
  Dma3(tempBuffer, &i, BUFFER_SIZE * sizeof(s16) / 4, DMA_MEMSET32);

  for (ch = 0; ch < 4; ch++) {
    if (channel[ch].isPlaying || channel[ch].fadeOut > 0) {
      for (i = 0; i < BUFFER_SIZE; i++) {
        s8 sample = 0;

        if (channel[ch].isPlaying) {
          sample = channel[ch].data[channel[ch].position];
          channel[ch].lastSample = sample;
          channel[ch].position++;

          if (channel[ch].position >= channel[ch].length) {
            if (channel[ch].looping) {
              while (channel[ch].position >= channel[ch].length) {
                channel[ch].position -= channel[ch].length;
              }
            } else {
              channel[ch].isPlaying = 0;
              channel[ch].fadeOut = 4; // 4 samples to fade out
            }
          }
        } else if (channel[ch].fadeOut > 0) {
          sample = channel[ch].lastSample * channel[ch].fadeOut / 4;
          channel[ch].fadeOut--;
        }
        tempBuffer[i] += sample;
      }
    }
  }

  for (i = 0; i < BUFFER_SIZE; i++) {
    targetBuffer[i] = (s8)tempBuffer[i];
  }
}

volatile u32 reload = 0;
volatile u32 fifoCounter = 0;

void isr(void) {
  fifoCounter++;

  if (fifoCounter >= 16) {
    reload = 1;
    if (mixbuf.activeBuffer == bufA) {
      DMA[1].control = 0;
      DMA[1].source = (u32)mixbuf.bufB;
      DMA[1].control =
          DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT | DMA_IRQ_ENABLE;
      mixbuf.activeBuffer = bufB;
    } else {
      DMA[1].control = 0;
      DMA[1].source = (u32)mixbuf.bufA;
      DMA[1].control =
          DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT | DMA_IRQ_ENABLE;
      mixbuf.activeBuffer = bufA;
    }
    fifoCounter = 0;
  }

  irqAcknowledge(IRQ_DMA1);
}

// only sets up direct sound A right now, begins reading from buffer
void initMonoFIFO() {
  ENABLE_AUDIO;
  AUDIO->ds = DS_MONO_INIT | DSA_TMR(0);

  mixbuf.activeBuffer = bufA;

  DMA[1].source = (u32)mixbuf.baseBuffer;
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT | DMA_IRQ_ENABLE;

  TIMER[0].value = 0xFBE8;
  TIMER[0].control = TMR_ENABLE;

  TIMER[1].value = (0xFFFF - 256);
  TIMER[1].control = TMR_ENABLE;

  ISR = isr;
  irqEnable(IRQ_DMA1);
}

void startMusicLoop() {
  channel[0].position = 0;
  channel[0].isPlaying = 1;
}

void stopMusicLoop() { channel[0].isPlaying = 0; }

void triggerKick() {
  channel[1].position = 0;
  channel[1].isPlaying = 1;
}

void triggerSnare() {
  channel[2].position = 0;
  channel[2].isPlaying = 1;
}

void triggerHat() {
  channel[3].position = 0;
  channel[3].isPlaying = 1;
}

int main(void) {
  DSPC = MODE3 | BG2;

  initMixChannels();
  irqMaster(ON);  // now enable interrupts
  initMonoFIFO(); // is now reading from buffer

  while (1) {
    if (reload == 1) {
      mixAudio();
      reload = 0;
    }

    updateKeys();

    if (keyTapped(ST)) {
      if (channel[0].isPlaying) {
        stopMusicLoop();
      } else {
        startMusicLoop();
      }
    }

    if (keyTapped(B)) {
      triggerKick();
    }

    if (keyTapped(A)) {
      triggerSnare();
    }

    if (keyTapped(RT)) {
      triggerHat();
    }

    if (keyTapped(LT)) {
      triggerHat();
    }
  }
  return 0;
}
