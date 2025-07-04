#include "../includes/directSound.h"
#include "audio/audio.h"
#include "audio/dmg.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "core/timer.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"

	//Formula for playback frequency: 0xFFFF - (cpuFreq/playbackFreq)

#define BUFFER_SIZE 256
#define ENABLE_AUDIO (AUDIO->master = AUDIO_MASTER_ENABLE)

typedef struct {
  const s8 *data;
  u32 position;
  u32 length;
} AudioChannel;

AudioChannel channel[4];

typedef enum { bufA = 0, bufB = 1 } ActiveBuffer;

typedef struct __attribute__((packed)) {
  union {
    struct {
      s8 bufA[BUFFER_SIZE];
      s8 bufB[BUFFER_SIZE];
    };
    s8 baseBuffer[BUFFER_SIZE * 2];
  };
  ActiveBuffer activeBuffer;
} Mixbuffer;

s8 singleBuffer[BUFFER_SIZE] = {0};
Mixbuffer mixbuf = {0};


void initMixChannels() {
  channel[0].data = tharp16k;   
  channel[0].position = 0;
  channel[0].length = tharp16klen;
}

void fillBuffer() {
  s8* targetBuffer = (mixbuf.activeBuffer == bufA) ? mixbuf.bufB : mixbuf.bufA;

  if (channel[0].position < channel[0].length - BUFFER_SIZE) {
    Dma3(targetBuffer, channel[0].data + channel[0].position, BUFFER_SIZE/4, DMA_MEMCPY32);
    channel[0].position += BUFFER_SIZE;
  } else {
    // End of sample - loop back to beginning
    channel[0].position = 0;
    Dma3(targetBuffer, channel[0].data, BUFFER_SIZE/4, DMA_MEMCPY32);
  }
}

volatile u32 reload = 0;

void tm_isr(void) {
  reload = 1;
  if (mixbuf.activeBuffer == bufA) {
    DMA[1].control = 0;
    DMA[1].source = (u32)mixbuf.bufB;
    DMA[1].control = DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT ; 
    mixbuf.activeBuffer = bufB;
  } else {
    DMA[1].control = 0;
    DMA[1].source = (u32)mixbuf.bufA;
    DMA[1].control = DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT; 
    mixbuf.activeBuffer = bufA;
  }
    
  irqAcknowledge(IRQ_TMR1);
}

//only sets up direct sound A right now, begins reading from buffer
void initMonoFIFO() {
  ENABLE_AUDIO;
  AUDIO->ds = DS_MONO_INIT | DSA_TMR(0);

  mixbuf.activeBuffer = bufA;

  DMA[1].source = (u32)mixbuf.baseBuffer;
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_ENABLE | DMA_START_SPECIAL; 

  TIMER[0].value = 0xFBE8;
  TIMER[0].control = TMR_ENABLE;				 

  TIMER[1].value = (0xFFFF - 256);
  TIMER[1].control = TMR_CASCADE | TMR_ENABLE | TMR_IRQ_ENABLE;

  ISR = tm_isr;
  irqEnable(IRQ_TMR1);
}

int main(void){
  DSPC = MODE3 | BG2;

  initMixChannels();
  irqMaster(ON);	// now enable interrupts
  fillBuffer();
  initMonoFIFO(); // is now reading from buffer

	while(1){
    if (reload == 1) {
      fillBuffer();
      reload = 0;
    }
  }
  return 0;
}
