#ifndef AUDIO_H
#define AUDIO_H

#include "typedefs.h"

// Sound register definitions
#define REG_SOUNDCNT_X  *(volatile u16*)0x4000084
#define REG_SOUNDCNT_L  *(volatile u16*)0x4000080
#define REG_SOUNDCNT_H  *(volatile u16*)0x4000082
#define REG_SOUND3CNT_L *(volatile u16*)0x4000070
#define SOUND3BANK32    0x0000
#define SOUND3SETBANK1  0x0040
#define REG_WAVE_RAM0   *(volatile u32*)0x4000090
#define REG_WAVE_RAM1   *(volatile u32*)0x4000094
#define REG_WAVE_RAM2   *(volatile u32*)0x4000098
#define REG_WAVE_RAM3   *(volatile u32*)0x400009C
#define SOUND3SETBANK0  0x0000
#define SOUND3PLAY      0x0080
#define REG_SOUND3CNT_H *(volatile u16*)0x4000072
#define SOUND3OUTPUT1   0x2000
#define REG_SOUND3CNT_X *(volatile u16*)0x4000074
#define SOUND3INIT      0x8000
#define SOUND3PLAYONCE  0x4000

void PlayNote(u16 frequency, unsigned char length);

#endif
