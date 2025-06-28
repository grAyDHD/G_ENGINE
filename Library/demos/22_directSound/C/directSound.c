#include "../includes/directSound.h"
#include "core/interrupts.h"

int main() {
  irqMaster(ON);

  while(1) {
    VBLANK();
  }
  return 0;
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
