#ifndef DMG_H
#define DMG_H

#include "typedefs.h"

#define SOUND_SYSTEM *(volatile u16 *)0x04000084
#define ENABLE_SOUND() (SOUND_SYSTEM |= 0x0080)
#define DISABLE_SOUND() (SOUND_SYSTEM &= 0xFF7F);
#define DMG_STEREO_OUTPUT *(volatile u16 *)0x04000080

#define DMG_1 *(volatile u16 *)0x04000060
#define SWEEP *(volatile u16 *)0x04000060
#define ENV *(volatile u16 *)0x04000062
#define FREQ *(volatile u16 *)0x04000064

#define SWEEP_RANGE(range) (DMG_1 = (DMG_1 & ~0x07) | ((8 - (range)) & 0x07))
#define SWEEP_TIME(time)                                                       \
  (DMG_1 = (DMG_1 & ~(0x07 << 4)) | (((8 - (time)) & 0x07) << 4))
#define SWEEP_DIRECTION(direction)                                             \
  (DMG_1 = (DMG_1 & ~(0x08)) | (direction << 3))

#define ENV_LENGTH // reset sound bit after set
#define ENV_DIRECTION

#define TIMED() (FREQ |= 0x4000)
#define CONTINUOUS() (FREQ &= ~0x4000)
#define PLAY() (FREQ |= 0x8000)
#define STOP() (FREQ &= ~0x8000)

void defaultCH1Sound();
void initSoundSystem();
void setCH1Sound(u8 sRange, u8 sTime, u8 sDirection, u8 envVolume, u8 envRate,
                 u8 envTone, u8 envDirection, u16 freqNote, u16 freqMode);
#endif // !DMG_H
