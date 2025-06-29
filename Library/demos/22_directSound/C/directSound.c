#include "core/typedefs.h"
#include "graphics/video.h"
#include "input/in.h"
#include "core/timer.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "audio/audio.h"
#include "../includes/FTGFull.h"

const u32* audioSample = (u32 *)FTG_raw;  //the sample, 8bit signed, 16Khz

void InterruptProcess(void) __attribute__ ((section(".iwram"))); //the interrupt handle from crt0.s
void DmaPlaySound (void);
void TimerPlaySound(void);

int iNextSample = 0;
// int SampleSize = 47312; 

void InterruptProcess(void){
  /*
    if(IRQ_FLAGS & IRQ_TMR0) {
    // is this best practice for acknowledging interrupts, have conditional of acknowledgement holding ISR code?
		//Interrupt mode Direct Sound IRQ
		//load FIFO each 4 samples with 4 bytes	
		if(!(iNextSample & 3)) {// 0000 0011
      //
      *FIFO_A=audioSample[iNextSample>>2];
    }

		iNextSample++;

		if(iNextSample>SampleSize){
			//sample finished!
      TIMER[0].control = 0; //disable timer
		}

	}else{
  */
		//Sample finished! Stop DMA mode Direct sound
		TIMER[0].control = 0;	//disable timer 0
		DMA[1].control = 0; //stop DMA			
	//}

  // this is a problem when have multiple interrupts, it clears all flags not just current
  IRQ_ACKNOWLEDGER |= IRQ_ACKNOWLEDGER; // irq flag raised, writes same irq flag to acknowledge/clear
}
 
int main(void){
//	play a sound using timer 0 as sampling source
//	When timer 0 overflows, the interrupt handler loads the FIFO with the next sample

  DSPC = MODE3 | BG2;

//	TimerPlaySound();  //play in interrupt mode

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
  ISR = InterruptProcess;

  AUDIO->dmg = 0;
  AUDIO->ds = DS_INIT | DSA_FIFO_RESET | DSB_FIFO_RESET;
  AUDIO->master = AUDIO_MASTER_ENABLE;
	
  DMA[1].source = (u32)audioSample;
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_REPEAT;
	
//  TIMER[1].value = 0x7098; // 0xffff - number of samples to play, use inline function with parameter of sample number
 // TIMER[1].control = TMR_ENABLE | TMR_IRQ_ENABLE | TMR_CASCADE;

 // irqEnable(IRQ_TMR1);
  //irqMaster(ON);
	
	//Formula for playback frequency is: 0xFFFF-round(cpuFreq/playbackFreq)
  TIMER[0].value = 0xFBE8; // 16khz playback freq
  TIMER[0].control = TMR_ENABLE; //enable at cpu freq?  What does this mean
				 
}

void TimerPlaySound(void){
	//Play a mono sound at 16khz in Interrupt mode Direct Sound
	//uses timer 0 as sampling rate source
  //ISR = InterruptProcess;

  AUDIO->ds = DSA_FULL_VOLUME | DSA_ENABLE_L | DSA_ENABLE_R;
  AUDIO->master = AUDIO_MASTER_ENABLE;
  // replace with audioMaster(ON | OFF)

  //irqEnable(IRQ_TMR0);
  //irqMaster(ON);

	//set playback frequency
	//note: using anything else than clock multipliers to serve as sample frequencies
	//		tends to generate distortion in the output. It has probably to do with timing and 
	//		FIFO reloading.

  TIMER[0].value = 0xFFFF; //shortest timer value, equal to cpu freq/scaler value, 1 tick to reset
  TIMER[0].control = TMR_FREQ_1024 | TMR_ENABLE | TMR_IRQ_ENABLE; // scaler sets 1 timer tick every 1024 cpu cycles? 
	
}

/*
int main() { 
  initializeVBI();
 // initializeTMRI();
  irqMaster(ON);
  soundInit();

  DSPC = MODE3 | BG2;
 
  while(1) {
    VBLANK();
    SndMix();
    updateKeys();

    if (keyTapped(LT)) {  
      sndChannel[0].data = 0; // clear channel
      sndChannel[0].pos = 0; //set to beginning?
      sndChannel[0].inc = 4096;
      sndChannel[0].vol = 64;
      sndChannel[0].length = 48419 << 12;
      sndChannel[0].loopLength = 0; // no loop
      sndChannel[0].data = (s8 *)guitarData;
    }
  }

    return 0;
}
*/

/*
#include "../includes/directSound.h"
#include "core/interrupts.h"
 
int main() {
  irqMaster(ON);
  initializeTMRI();
  

  while(1) {
    VBLANK();
  }
  return 0;
}
 
*/

