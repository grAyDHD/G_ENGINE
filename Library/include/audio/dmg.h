#ifndef DMG_H
#define DMG_H

#include "core/typedefs.h"
// FEDC BA08 7654 3210
// Master sound enable and dmg 1-4 play status
#define SOUND_SYSTEM *(volatile u16 *)0x04000084
#define ENABLE_SOUND() (SOUND_SYSTEM |= 0x0080)
#define DISABLE_SOUND() (SOUND_SYSTEM &= 0xFF7F);

// 0-2 DMG Left vol, 3: vin/unused, 4-6 DMG Right vol, 7: vin/unused
// 89AB DMG 1-4 Left enable; CDEF DMG 1-4 Right enable
#define DMG_STEREO_OUTPUT *(volatile u16 *)0x04000080
#define MAX_OUTPUT() (DMG_STEREO_OUTPUT |= FF77)

// 0-2 sweep shifts 3 sweepdirection, 0 up, 1 down
// 4-6 sweep time, 0 off, 001 7.8ms, 010, 15.6 ms, etc. time between each shift
#define DMG_1 *(volatile u16 *)0x04000060
#define ENV_1 *(volatile u16 *)0x04000062
#define FREQ_1 *(volatile u16 *)0x04000064
// value 0-7
#define SWEEP_RANGE(range) (DMG_1 = (DMG_1 & ~0x07) | ((8 - (range)) & 0x07))
// value 0-7
#define SWEEP_TIME(time)                                                       \
  (DMG_1 = (DMG_1 & ~(0x07 << 4)) | (((8 - (time)) & 0x07) << 4))
// 0 up, 1 down
#define SWEEP_DIRECTION(direction)                                             \
  (DMG_1 = (DMG_1 & ~(0x08)) | (direction << 3))

// LENGTH, wave duty cycle, envelope control
#define ENV_LENGTH    // resets sound bit after set (sound length =
                      // (64-registerValue)*(1/256) seconds)
#define ENV_SWELL     // bit B = 1
#define ENV_FADE      // bit B = 0
#define ENV_TONE(val) // bits 6-7, 00 = 12.5%, 01=25%, 10=50%, 11=75%, val 1-4
#define ENV_VOLUME    // volume envolope STARTS at

#define TIMED() (FREQ_1 |= 0x4000)
// in continuous, alwasy set length to zero after freq change
#define CONTINUOUS() (FREQ_1 &= ~0x4000)
#define PLAY() (FREQ_1 |= 0x8000)
#define STOP() (FREQ_1 &= ~0x8000)

void defaultCH1Sound();
void initSoundSystem();
void setCH1Sound(u8 sRange, u8 sTime, u8 sDirection, u8 envVolume, u8 envRate,
                 u8 envTone, u8 envDirection, u16 freqNote, u16 freqMode);
#endif // !DMG_H
