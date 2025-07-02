#include "core/typedefs.h"
#include "graphics/video.h"
#include "input/in.h"
#include "core/timer.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "audio/audio.h"
#include "../includes/k16.h"
#include "../includes/k8.h"
#include "../includes/s16.h"
#include "../includes/s8.h"
#include "../includes/tharp16.h"
#include "../includes/tharp16.h"


// todo: move audio data to c files and out of headers
s8 mixBufferA[256];
s8 mixBufferB[256];
// audio variables from headers:
// s8 []: kick16k same for snare and tharp, 16k and 8k versions for each
// u32: kick16klen

typedef struct {
  // data, pointer to start of audio sample *u32?
  // length = samplelen u32, u16 not large enough to contain length of longer samples
  // position? if position is 0, not playing, else playing- u32
  // use highest bit of position to represent whether looping or not, mask that bit for position calculations
} DMAAudioChannel;
// seems like too many members, will narrow down to minimum necessary while retaining flexibility/control

void startAudioLoop(void);
void stopAudioLoop(void);
void tmr1_isr(void);
 
int main(void){
  DSPC = MODE3 | BG2;

	while(1){
    updateKeys();

    //--- Program Flow ---//
		if(keyTapped(ST)){
      startAudioLoop();
      // if tharp loop is playing, stop
      // if not, start playback
		}
	}
}


/* will rewrite this
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

void startAudioLoop (void){
  // this isn't quite right, but how to loop it? first i'll get it to just play
//  ISR = tmr1_isr;

  AUDIO->ds = DS_MONO_INIT | DSA_FIFO_RESET | DSA_TMR(0);
  AUDIO->master = AUDIO_MASTER_ENABLE;
  
  // create separate MixBuffer struct? with bufferA, bufferB?a	
//  DMA[1].source = (u32)mixBufferA; //irq will set source to buffer b?
  //buffer A and B should be back to back, mix buffer struct? this will mimic tutorial of only swapping buffers when buffer B is finished being used by dma, so interrupt could be every 128 samples?  
  // 32 bits, is that four samples?  256/4 = 64 dma transfers per buffer
  DMA[1].source = (u32)tharp16k;
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_ENABLE | DMA_32BIT | DMA_START_SPECIAL | DMA_REPEAT;
	

	//Formula for playback frequency: 0xFFFF - (cpuFreq/playbackFreq)
  TIMER[0].value = 0xFBE8; // 16khz playback freq
  TIMER[0].control = TMR_ENABLE;				 

//  irqEnable(IRQ_TMR1);
 // irqMaster(ON);
	
  //for shorter audio, 0xFFFF - number of samples and stop on irq
  TIMER[1].value = 0xFFFF - 256; //for 256 sample buffer, isr must handle buffer swap
  TIMER[1].control = TMR_ENABLE | TMR_IRQ_ENABLE | TMR_CASCADE;
}
