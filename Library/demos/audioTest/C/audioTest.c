#include "core/typedefs.h"
#include "graphics/video.h"
#include "input/in.h"
#include "core/timer.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "audio/audio.h"

extern const u32 _binary_lo1234_pcm_start[];  //the sample, 8bit signed, 16Khz

void InterruptProcess(void) __attribute__ ((section(".iwram"))); //the interrupt handle from crt0.s
void DmaPlaySound (void);
void TimerPlaySound(void);

int iNextSample=0;
int SampleSize=36712; 

/* 
void InterruptProcess(void){
	if(REG_IF & INT_TIMER0){
		//Interrupt mode Direct Sound IRQ
		//load FIFO each 4 samples with 4 bytes	
		if(!(iNextSample&3))REG_SGFIFOA=_binary_lo1234_pcm_start[iNextSample>>2];

		iNextSample++;

		if(iNextSample>SampleSize){
			//sample finished!
			REG_TM0CNT_H=0;	//disable timer 0
		}

	}else{
		//Sample finished! Stop DMA mode Direct sound
		REG_TM0CNT_H=0;	//disable timer 0
		REG_DMA1CNT_H=0; //stop DMA		
	
	}
	//clear the interrupt(s)
	REG_IF |= REG_IF;
}
*/
 
int AgbMain(void){
//	play a sound using timer 0 as sampling source
//	When timer 0 overflows, the interrupt handler loads the FIFO with the next sample

  DSPC = MODE3 | BG2;

	TimerPlaySound();  //play in interrupt mode

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

//	REG_SOUNDCNT_L=0;
//	REG_SOUNDCNT_H=0x0b0F;  //DS A&B + fifo reset + timer0 + max volume to L and R
//	REG_SOUNDCNT_X=0x0080;  //turn sound chip on
  AUDIO->dmg = 0;
  AUDIO->ds=0x0b0F;
  AUDIO->master = AUDIO_MASTER_ENABLE;
	
	REG_DMA1SAD=(unsigned long)_binary_lo1234_pcm_start;	//dma1 source
	REG_DMA1DAD=0x040000a0; //write to FIFO A address
	REG_DMA1CNT_H=0xb600;	//dma control: DMA enabled+ start on FIFO+32bit+repeat
	
	REG_TM1CNT_L=0x7098;	//0xffff-the number of samples to play
	REG_TM1CNT_H=0xC4;		//enable timer1 + irq and cascade from timer 0

	REG_IE=0x10;	  	//enable irq for timer 1 overflow
	REG_IME=1;				//enable interrupt
	
	//Formula for playback frequency is: 0xFFFF-round(cpuFreq/playbackFreq)
	REG_TM0CNT_L=0xFBE8;	//16khz playback freq
	REG_TM0CNT_H=0x0080; 	//enable timer at CPU freq 
				 
}

void TimerPlaySound(void){
	//Play a mono sound at 16khz in Interrupt mode Direct Sound
	//uses timer 0 as sampling rate source

	REG_SOUNDCNT_H=0x0B0F;  //DirectSound A + fifo reset + max volume to L and R
	REG_SOUNDCNT_X=0x0080;  //turn sound chip on

	REG_IE=0x8;		//enable timer 0 irq
	REG_IME=1;				//enable interrupts

	//set playback frequency
	//note: using anything else thank clock multipliers to serve as sample frequencies
	//		tends to generate distortion in the output. It has probably to do with timing and 
	//		FIFO reloading.

	REG_TM0CNT_L=0xffff; 
	REG_TM0CNT_H=0x00C3;	//enable timer at CPU freq/1024 +irq =16386Khz sample rate
	
}
