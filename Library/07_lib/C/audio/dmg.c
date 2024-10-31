#include "dmg.h"
#include "typedefs.h"

void initSoundSystem() {
  ENABLE_SOUND();
  DMG_STEREO_OUTPUT = 0xFF77;
}

void defaultCH1Sound() {
  // SWEEP_RANGE(4);     // 0 1 2 3 4 5 6 7
  // SWEEP_TIME(4);      // 0 7 6 5 4 3 2 1
  // SWEEP_DIRECTION(1); // direction up

  // ENV_1 = 0xF000;  // Max length, 50% duty cycle, fast step, increment
  // CONTINUOUS
  FREQ_1 = 0x04D6; // set frequency
}

void setCH1Sound(u8 sRange, u8 sTime, u8 sDirection, u8 envVolume, u8 envRate,
                 u8 envTone, u8 envDirection, u16 freqNote, u16 freqMode) {
  SWEEP_RANGE(sRange);
  SWEEP_TIME(sTime);
  SWEEP_DIRECTION(sDirection);
};
// dmgSetup(&DMG_1, 3, 3, 10, 3, 2, A, 0);
