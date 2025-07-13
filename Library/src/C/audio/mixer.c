#include "../../include/audio/mixer.h"
#include "core/dma.h"

AudioChannel channel[4];

Mixbuffer mixbuf = {0};

void initMixChannels() {
  // Channel 0: Music loop (tharp)
  for (int i = 0; i < MAX_CHANNELS; i++) {
    channel[i].data = 0;
    channel[i].position = 0;
    channel[i].length = 0;
    channel[i].volume = 0;
    channel[i].isPlaying = 0;
    channel[i].looping = 0;
    channel[i].fadeOut = 0;
    channel[i].lastSample = 0;
  }
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
