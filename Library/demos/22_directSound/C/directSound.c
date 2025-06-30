#include "core/typedefs.h"
#include "graphics/video.h"
#include "input/in.h"
#include "core/timer.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "audio/audio.h"
#include "../includes/FTGFull.h"

const u32* audioSample = (u32 *)FTG_raw;  //the sample, 8bit signed, 16Khz

void DmaPlaySound (void);
 
int main(void){
//	play a sound using timer 0 as sampling source
//	When timer 0 overflows, the interrupt handler loads the FIFO with the next sample

  DSPC = MODE3 | BG2;

	while(1){
    updateKeys();
		if(keyTapped(A | B)){
			TIMER[0].control=0;	//disable timer 0
			DmaPlaySound(); //play in dma mode
		}
	};
}


void DmaPlaySound (void){
	//Play a mono sound at 16khz in DMA mode Direct Sound
	//uses timer 0 as sampling rate source
	//uses timer 1 to count the samples played in order to stop the sound 
//  ISR = InterruptProcess;

  AUDIO->ds = DS_INIT | DSA_FIFO_RESET | DSB_FIFO_RESET;
  AUDIO->master = AUDIO_MASTER_ENABLE;
	
  DMA[1].source = (u32)audioSample;
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_REPEAT;
	
//  TIMER[1].value = 0x7098; // 0xffff - number of samples to play, use inline function with parameter of sample number
//  TIMER[1].control = TMR_ENABLE | /* TMR_IRQ_ENABLE |*/ TMR_CASCADE;

 // irqEnable(IRQ_TMR1);
 // irqMaster(ON);
	
	//Formula for playback frequency is: 0xFFFF-round(cpuFreq/playbackFreq)
  TIMER[0].value = 0xFBE8; // 16khz playback freq
  TIMER[0].control = TMR_ENABLE; //enable at cpu freq?  What does this mean
				 
}
