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
  u16 pcm; // 0x04000082
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
#define DSA_TMR_SELECT(tmr) (tmr << 10) // only timer 0 or 1
#define DSA_FIFO_RESET (1 << 11)

#define DSB_ENABLE_R (1 << 12)
#define DSB_ENABLE_L (1 << 13)
#define DSB_TMR_SELECT(tmr) (tmr << 14) // only timer 0 or 1
#define DSB_FIFO_RESET (1 << 15)

//--- Master sound controls ---//
// sndcnt_x
//0400084
// bits 0-3 read only dmg play status (one if corresponding ch is active)
#define AUDIO_MASTER_ENABLE (1 << 7) //bit 7 enable/disable
#define AUDIO_MASTER_DISABLE (0)

// 040000A0 FIFO_A_L sound A fifo dat 0 and data 1
// 040000A2 FIFO_A_H sound a fifo Data 2 and data 3
// 040000A4 FIFO_B_L sound B fifo data 0 and 1
// 040000A6 FIFO_B_H sound B fifo data 2 and 3``

void dsAudioInit(){
  AUDIO->pcm = DSA_FULL_VOLUME | DSA_ENABLE_L | DSA_ENABLE_R | DSA_FIFO_RESET;
  //todo: this should likely be separate with only a single bit to toggle.
  AUDIO->master = AUDIO_MASTER_ENABLE;
}


#endif
