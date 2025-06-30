#include "core/typedefs.h"
#include "graphics/video.h"
#include "input/in.h"
#include "core/timer.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "audio/audio.h"
#include "../includes/FTGFull.h"

const u32* audioSample = (u32 *)FTG_raw;  //the sample, 8bit signed, 16Khz
void DmaPlaySound(void);
void tmr1_isr(void);
 
int main(void){
  DSPC = MODE3 | BG2;

	while(1){
    updateKeys();
		if(keyTapped(A | B)){
			TIMER[0].control = 0;	//disable timer 0
			DmaPlaySound(); //play in dma mode
		}
	};
}

int overflow = 0;
void tmr1_isr() {
  if (overflow == 43) {
    DMA[1].control = 0;
    TIMER[0].control = 0;
    TIMER[1].control = 0;
    overflow = 0;
  } else {
    overflow++;
  }
  irqAcknowledge(IRQ_TMR1);
}

void DmaPlaySound (void){
	//Play a mono sound at 16khz in DMA mode Direct Sound
	//uses timer 0 as sampling rate source
	//uses timer 1 to count the samples played in order to stop the sound 
  ISR = tmr1_isr;

  AUDIO->ds = DS_INIT | DSA_FIFO_RESET | DSB_FIFO_RESET;
  AUDIO->master = AUDIO_MASTER_ENABLE;
	
  DMA[1].source = (u32)audioSample;
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_REPEAT;
	
  //for shorter audio, 0xFFFF - number of samples
  TIMER[1].value = 0;
  TIMER[1].control = TMR_ENABLE | TMR_IRQ_ENABLE | TMR_CASCADE;

  irqEnable(IRQ_TMR1);
  irqMaster(ON);
	
	//Formula for playback frequency: 0xFFFF - (cpuFreq/playbackFreq)
  // cpuFreq = 2^24
  TIMER[0].value = 0xFBE8; // 16khz playback freq
  TIMER[0].control = TMR_ENABLE;				 
}
