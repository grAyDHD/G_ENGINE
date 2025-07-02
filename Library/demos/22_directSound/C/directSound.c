#include "../includes/directSound.h"
#include "core/dma.h"

u8 singleBuffer[256] = {0};
u32 tharpPosition = 0;

void isr(void) {
  // Fill buffer with next chunk of audio
  if (tharpPosition < tharp16klen - 256) {
    Dma3(singleBuffer, tharp16k + tharpPosition, 64, DMA_MEMCPY32);
    tharpPosition += 256;
  } else {
    // End of sample - could loop or stop
    tharpPosition = 0;  // Loop back to beginning
    Dma3(singleBuffer, tharp16k, 64, DMA_MEMCPY32);
    tharpPosition += 256;
  }
  
  // Restart DMA to play the newly filled buffer
  DMA[1].source = (u32)singleBuffer;
  DMA[1].control = DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_IRQ_ENABLE;
  irqAcknowledge(IRQ_DMA1);
}

int main(void){
  DSPC = MODE3 | BG2;

  // prefill buffer with audio
  Dma3(singleBuffer, tharp16k + tharpPosition, 64, DMA_MEMCPY32);
  tharpPosition += 256;

  // setup interrupts/dma1
  ISR = isr;
  irqEnable(IRQ_DMA1);
  irqMaster(ON);	
  
  // Initiate audio playback
  AUDIO->ds = DS_MONO_INIT | DSA_TMR(0);
  AUDIO->master = AUDIO_MASTER_ENABLE;
  DMA[1].source = (u32)singleBuffer;
  DMA[1].wordCount = 64; 
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_IRQ_ENABLE;
  
	//Formula for playback frequency: 0xFFFF - (cpuFreq/playbackFreq)
  TIMER[0].value = 0xFBE8; // 16khz playback freq, controls DMA repeat rate
  TIMER[0].control = TMR_ENABLE;				 

	while(1){

  
  }
  return 0;
}
