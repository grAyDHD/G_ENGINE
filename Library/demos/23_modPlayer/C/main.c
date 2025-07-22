#include "../includes/23_modPlayer.h"
#include "audio/mixer.h"
#include "input/in.h"

int main() {
  s32 i;
  int x = 0, y = 0;
  i = 0;

  DSPC = MODE3 | BG2;
  initCharLookup();

  /*

  //  initialize main sound variables
  sndVars.curMixBuffer = sndVars.mixBufferBase;
  sndVars.mixBufferSize = BUFFER_SIZE;
  sndVars.mixFreq = 16000;
  sndVars.rcpMixFreq = (1 << 28) / 16000;
  sndVars.samplesUntilMODTick = 0;
  sndVars.samplesPerMODTick = 0;
  sndVars.activeBuffer = 0; // 1 so first swap will start DMA

  i = 0;
  Dma3(sndMixBuffer, &i, 736 * 2 / 4, DMA_MEMSET32);

  // initialize channel structures
  for (i = 0; i < 4; i++) {
    channel[i].data = 0;
    channel[i].position = 0;
    channel[i].increment = 0;
    channel[i].volume = 0;
    channel[i].length = 0;
    channel[i].loopLength = 0;
  }
*/
  initMixChannels();

  irqMaster(ON);  // now enable interrupts
  initMonoFIFO(); // is now reading from buffer

  initDebugDisplay();

  while (1) {
    if (reload == 1) {
      //      mixAudio();
      SndUpdate();
      reload = 0;
    }

    x++;
    y += 2;

    VBLANK();
    //    updateDebugNumbers(x, y, 0);

    if (KEY_DOWN(ST)) {
      SndPlayMOD(MOD_AdventureBoy);
    }
  }
  return 0;
}
