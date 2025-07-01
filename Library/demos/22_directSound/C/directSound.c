#include "core/typedefs.h"
#include "graphics/video.h"
#include "input/in.h"
#include "core/timer.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "audio/audio.h"
//#include "../includes/FTG_L.h"
//#include "../includes/FTG_R.h"

// todo: move audio data to c files and out of headers
s8 mixBufferA[64];
s8 mixBufferB[64];

const u32* audioLeft = (u32 *)FTG_L_raw;  //the sample, 8bit signed, 16Khz
const u32* audioRight = (u32 *)FTG_R_raw;  //the sample, 8bit signed, 16Khz
void DmaPlaySound(void);
void tmr1_isr(void);
 
int main(void){
  DSPC = MODE3 | BG2;

	while(1){
    updateKeys();
    // create bgPlaying var?  if playing, stop, if not, start.
		if(keyTapped(ST)){
			DmaPlaySound(); //rename function, will play a full song
		}
	}
}


/*
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
*/

void DmaPlaySound (void){
//  ISR = tmr1_isr;

  AUDIO->ds = DS_STEREO_INIT | DSA_FIFO_RESET | DSB_FIFO_RESET | DSA_TMR(0) | DSB_TMR(0);
  AUDIO->master = AUDIO_MASTER_ENABLE;
  
	
  DMA[1].source = (u32)audioLeft;
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_REPEAT;
	
  DMA[2].source = (u32)audioRight;
  DMA[2].destination = (u32)FIFO_B;
  DMA[2].control = DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_REPEAT;


	//Formula for playback frequency: 0xFFFF - (cpuFreq/playbackFreq)
  TIMER[0].value = 0xFBE8; // 16khz playback freq
  TIMER[0].control = TMR_ENABLE;				 

  irqEnable(IRQ_TMR1);
  irqMaster(ON);
	
 // TIMER[1].value = 0; //for shorter audio, 0xFFFF - number of samples and stop on irq
  TIMER[1].value = 0xFFFF - 128;
  TIMER[1].control = TMR_ENABLE | TMR_IRQ_ENABLE | TMR_CASCADE;
}
