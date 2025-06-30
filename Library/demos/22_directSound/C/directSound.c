#include "core/typedefs.h"
#include "graphics/video.h"
#include "input/in.h"
#include "core/timer.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "audio/audio.h"
#include "../includes/FTG_L.h"
#include "../includes/FTG_R.h"

// todo: create interface for song selection, pre process songs to play well with gba audio make less muddy
// todo: move audio data to c files and out of headers
const u32* audioLeft = (u32 *)FTG_L_raw;  //the sample, 8bit signed, 16Khz
const u32* audioRight = (u32 *)FTG_R_raw;  //the sample, 8bit signed, 16Khz
void DmaPlaySound(void);
void tmr1_isr(void);
 
int main(void){
  DSPC = MODE3 | BG2;

	while(1){
    updateKeys();
		if(keyTapped(A | B)){
			DmaPlaySound(); //play in dma mode
		}
	}
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

  AUDIO->ds = DS_STEREO_INIT | DSA_FIFO_RESET | DSB_FIFO_RESET | DSA_TMR(0) | DSB_TMR(0);
  AUDIO->master = AUDIO_MASTER_ENABLE;
	
  DMA[1].source = (u32)audioLeft;
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_REPEAT;
	
  DMA[2].source = (u32)audioRight;
  DMA[2].destination = (u32)FIFO_B;
  DMA[2].control = DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_REPEAT;

  TIMER[1].value = 0; //for shorter audio, 0xFFFF - number of samples and stop on irq
  TIMER[1].control = TMR_ENABLE | TMR_IRQ_ENABLE | TMR_CASCADE;

  irqEnable(IRQ_TMR1);
  irqMaster(ON);
	
	//Formula for playback frequency: 0xFFFF - (cpuFreq/playbackFreq)
  TIMER[0].value = 0xFBE8; // 16khz playback freq
  TIMER[0].control = TMR_ENABLE;				 
}
