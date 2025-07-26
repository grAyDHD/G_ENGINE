#ifndef MIXER_H
#define MIXER_H

#include "core/typedefs.h"

// Formula for playback frequency: 0xFFFF - (cpuFreq/playbackFreq)
#define BUFFER_SIZE 256
#define MAX_CHANNELS 4

typedef struct {
  const s8 *data;
  u32 position;
  u32 length;
  u32 increment; // fixed point increment, 4096 = normal
  u32 loopLength;
  u32 volume;
  // u16 isPlaying;
  // u16 looping;
  // u16 fadeOut;

  // s8 lastSample;
} AudioChannel;

extern AudioChannel channel[4];

typedef enum { bufA = 0, bufB = 1 } ActiveBuffer;

typedef struct __attribute__((packed)) {
  union {
    struct {
      s8 bufA[BUFFER_SIZE];
      s8 bufB[BUFFER_SIZE];
    };
    s8 bufBase[BUFFER_SIZE * 2];
  };
  ActiveBuffer activeBuffer;
  s8 *position;
} Mixbuffer;

extern Mixbuffer mixbuf;
extern volatile u32 reload;

void audioIsr(void);
void initMixChannels();
void initMonoFIFO();
void swapMixBuffer();
void mixAudio();

#endif // !MIXER_H
