#include "../includes/directSound.h"

const u8 singleBuffer[256] = {0};

void isr(void) {
  DMA[1].source = (u32)singleBuffer; // reads 4 samples at once from DMA, so must increment position in buffer.
  DMA[1].source = (u32)singleBuffer;
  DMA[1].wordCount = 64; 
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_IRQ_ENABLE;
  irqAcknowledge(IRQ_DMA1);
}

int main(void){

  ISR = isr;
  irqEnable(IRQ_DMA1);
  irqMaster(ON);	
  
  DSPC = MODE3 | BG2;

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
