#ifndef AUDIO_H
#define AUDIO_H

//--- DMG Sound Controls ---//
// sound control L ch LR volume/enable
// bit 0-2 sound 1-4 master volume right
// bit 3 unused
// bit 4-6 sound 1-4 master volume left
// bit 7 unused
// 8-11 sound 1-4 enable flags RIGHT
// 12-15 sound 1-4 enable flags LEFT

//--- Direct Sound Controls ---//
// 04000082
// DMA Sound control/mixing
// bit 0-1 dmg volume? 0=25% 1-50% 2=100% no 3
// bit 2 DMA ch A Volume 0=50% 1=100%
// bit 3 DMA ch B Volume 0=50% 1=100%
// bit 4-7 unused
// bit 8 DMA sound A enable RIGHT
// bit 9 DMA sound A enable LEFT
// bit 10 DMA Sound A timer select (0 or 1)
// bit 11 DMA Sound A Reset FIFO (1 = reset)
// bit 12 DMA sound B enable RIGHT
// bit 13 DMA sound B enable LEFT
// bit 14 DMA Sound B timer select (0 or 1)
// bit 15 DMA Sound B Reset FIFO (1 = reset)

//--- Maser sound controls ---//
//0400084
//Sound on/off
//bits 0-3 read only dmg 1-4 play status
//bits 4-6 not used
//bit 7 psg/fifo master enable, psg reset to 0 on cleark 82 and 88 stay R/W though 82 doesn't do anything when sound is off
//bit 8-31 unused

//--- Sound Bias ---//
// Sound PWM control
// bit 0 unused
// bits 1-9 bias level (default = 100h,converting signed samples into unsigned)
// bits 10-13 not used
// bits 14-15 Amplitude Resolution/sampling cycle
// bits 16-31 not used
// Amplitude Resolution/Sampling Cycle (0-3)
// 0 9bit / 32.768kHz default, best for dma A/B
// 1 8 bit / 65.536 kHz
// 2 7 bit /131.072 kHz
// 3 6bit / 262.144 kHz, best for PSG 1-4

// 400008Ch - Not used
// 400008Eh - Not used

// 040000A0 FIFO_A_L sound A fifo dat 0 and data 1
// 040000A2 FIFO_A_H sound a fifo Data 2 and data 3
// 040000A4 FIFO_B_L sound B fifo data 0 and 1
// 040000A6 FIFO_B_H sound B fifo data 2 and 3``
//

#endif
