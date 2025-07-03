#include "../includes/directSound.h"
#include "core/dma.h"

u8 singleBuffer[16] = {0};

typedef struct {
  const s8 *data;
  u32 position;
  u32 length;
} AudioChannel;

AudioChannel channel[4];

void initAudioChannel() {
  channel[0].data = tharp16k;   
  channel[0].position = 0;
  channel[0].length = tharp16klen;
}

void isr(void) {
  // Fill buffer with next chunk of audio
  if (channel[0].position < channel[0].length - 16) {
    //fill buffer with audio from audio's current position
    Dma3(singleBuffer, tharp16k + channel[0].position, 4, DMA_MEMCPY32);
    channel[0].position += 16;
  } else {
    // End of sample - could loop or stop
    channel[0].position = 0;  // Loop back to beginning
    Dma3(singleBuffer, tharp16k, 4, DMA_MEMCPY32);
  }
  
  DMA[1].control = DMA_ENABLE | DMA_START_SPECIAL | DMA_IRQ_ENABLE;
  irqAcknowledge(IRQ_DMA1);
}

int main(void){
  DSPC = MODE3 | BG2;

  initAudioChannel();

  // setup interrupts/dma1
  ISR = isr;
  irqEnable(IRQ_DMA1);
  irqMaster(ON);	
  
  // Initiate audio playback: FIFO mode ignores word count and transfer type 16/32 bit, 32 bit copies
  AUDIO->ds = DS_MONO_INIT | DSA_TMR(0);
  AUDIO->master = AUDIO_MASTER_ENABLE;
  DMA[1].source = (u32)singleBuffer;
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_ENABLE | DMA_START_SPECIAL | DMA_IRQ_ENABLE | DMA_REPEAT;
  
	//Formula for playback frequency: 0xFFFF - (cpuFreq/playbackFreq)
  TIMER[0].value = 0xFBE8; // 16khz playback freq, controls DMA repeat rate
  TIMER[0].control = TMR_ENABLE;				 

	while(1){

  
  }
  return 0;
}
