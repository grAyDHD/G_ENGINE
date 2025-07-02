#ifndef AUDIO_H
#define AUDIO_H
#include "core/typedefs.h"

//--- DMG Sound Controls ---//
// 04000080

#define DMG_VOLUME_R(v) (v << 0) // max = 7
#define DMG_VOLUME_L(v) (v << 4) // max = 7
#define ENABLE_DMG_RIGHT(ch) ((1 << 8) << (ch - 1)) // 8-11 sound 1-4 enable flags RIGHT
#define ENABLE_DMG_LEFT(ch) ((1 << 12) << (ch - 1)) // 12-15 sound 1-4 enable flags LEFT
//add disable macros

typedef struct {
  u16 dmg; // 0x04000080
  u16 ds; // 0x04000082
  u16 master; // 0x04000084
} AudioControls;

#define AUDIO ((AudioControls *)0x04000080)

//--- Direct Sound Controls ---//
// 04000082
// DS Sound control/mixing
typedef enum {QUARTER = 0, HALF, FULL} Ratio;

#define DMG_RATIO(r) (r) //enum quarter = 0, half = 1 full = 2
#define DSA_HALF_VOLUME (0 << 2)
#define DSA_FULL_VOLUME (1 << 2)
#define DSB_HALF_VOLUME (0 << 3)
#define DSB_FULL_VOLUME (1 << 3)

#define DSA_ENABLE_R (1 << 8)
#define DSA_ENABLE_L (1 << 9)
#define DSA_TMR(tmr) (tmr << 10) // only timer 0 or 1
#define DSA_FIFO_CLEAR (1 << 11)

#define DSB_ENABLE_R (1 << 12)
#define DSB_ENABLE_L (1 << 13)
#define DSB_TMR(tmr) (tmr << 14) // only timer 0 or 1
#define DSB_FIFO_CLEAR (1 << 15)

#define DS_MONO_INIT (DSA_ENABLE_L | DSA_ENABLE_R | DSB_ENABLE_L | DSB_ENABLE_R | DSA_FULL_VOLUME | DSB_FULL_VOLUME | DSA_FIFO_CLEAR | DSB_FIFO_CLEAR)
#define DS_STEREO_INIT (DSA_ENABLE_L | DSB_ENABLE_R | DSA_FULL_VOLUME | DSB_FULL_VOLUME)

//--- Master sound controls ---//
// sndcnt_x
//0400084
// bits 0-3 read only dmg play status (one if corresponding ch is active)
#define AUDIO_MASTER_ENABLE (1 << 7) //bit 7 enable/disable
#define AUDIO_MASTER_DISABLE (0)

#define FIFO_A ((volatile u32 *)0x040000A0) // 4x8 bit chunk transfers
#define FIFO_B ((volatile u32 *)0x040000A4)

void dsAudioInit(){
  AUDIO->ds = DSA_FULL_VOLUME | DSA_ENABLE_L | DSA_ENABLE_R | DSA_FIFO_CLEAR;
  //todo: this should likely be separate with only a single bit to toggle.
  AUDIO->master = AUDIO_MASTER_ENABLE;
}

//DS in DMA mode:
//set DS outputs and volumes
//set timer 0 or 1 count value to 0xffff-round(cpuFreq/playbackFreq)
  // 65536 = (0xffff, unsigned)
  // 65536 - round(2^24/16000)
// set DMA source to sample address and dest to fifo a or b
// fifo reset bit before starting sound
// set dma 1 or 2 start mode to 11 (special?)
  // this sets dma to repeat on fifo empty? some docs say prohibited
// set dma repeat and 32 bit moves and set src/dest to increment
// enable timer at cpu frequency (prescaler/clock divider = 0)

#endif
