#include "../includes/directSound.h"
#include "audio/audio.h"
#include "audio/dmg.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "core/timer.h"

	//Formula for playback frequency: 0xFFFF - (cpuFreq/playbackFreq)

#define BUFFER_SIZE 256
#define ENABLE_AUDIO (AUDIO->master = AUDIO_MASTER_ENABLE)

typedef struct {
  const s8 *data;
  u32 position;
  u32 length;
} AudioChannel;

AudioChannel channel[4];

typedef struct __attribute__((packed)) {
  s8 bufA[BUFFER_SIZE];
  s8 bufB[BUFFER_SIZE];
} Mixbuffer;

void initMixChannels() {
  channel[0].data = tharp16k;   
  channel[0].position = 0;
  channel[0].length = tharp16klen;
}

Mixbuffer dualBuffer = {0};

enum CurrentBuffer { bufA = 0, bufB = 1};
enum CurrentBuffer currentBuffer = bufA;


  /*
   * this fills buffer with channel 0 data.
  if (channel[0].position < channel[0].length - BUFFER_SIZE) {
    Dma3(singleBuffer, channel[0].data + channel[0].position, BUFFER_SIZE/4, DMA_MEMCPY32);
    channel[0].position += BUFFER_SIZE;
  } else {
    // End of sample - loop back to beginning
    channel[0].position = 0;
    Dma3(singleBuffer, channel[0].data, BUFFER_SIZE/4, DMA_MEMCPY32);
  }
  */

void tm_isr(void) {
  if (currentBuffer == bufA) { // buffer A just finished
    currentBuffer = bufB;
    DMA[1].source = bufB;


  } else { // buffer B just finished
    currentBuffer = bufA;
    DMA[1].source = bufA;
    // fill buffer B, play buffer A
    
  }

  DMA[1].control = 0;  // Disable completely  
  DMA[1].control = DMA_ENABLE | DMA_START_SPECIAL | DMA_REPEAT | DMA_DEST_FIXED;  // Full restart
    
  irqAcknowledge(IRQ_TMR1);
}

//only sets up direct sound A right now, begins reading from buffer
void initMonoFIFO() {
  ENABLE_AUDIO;
  AUDIO->ds = DS_MONO_INIT | DSA_TMR(0);

  //fill buffers A and B
  Dma3(dualBuffer.bufA, channel[0].data, 128, DMA_MEMCPY32); // 128 words, 512 bytes 
  channel[0].position += 512; // advance 512 bytes/samples

  DMA[1].source = (u32)dualBuffer.bufA;
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
  initMonoFIFO(); // is now reading from buffer
  irqMaster(ON);	// timer interrupt every 256 samples
  
	while(1){}
  return 0;
}
