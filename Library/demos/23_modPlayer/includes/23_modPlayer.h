#ifndef MODPLAYER_H
#define MODPLAYER_H

#include "../data/dataSndData.h"
#include "audio/mixer.h"
#include "core/dma.h"
#include "core/interrupts.h"
#include "core/timer.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"
#include "input/in.h"

// samplesPerTick = mixFreq / MODFreq
// MODFreq = tempo in Hz, where hz = tempo * 2/5
// so, if tempo equals 120 bpm:
// 120/(2/5) = 300
// mixFreq = sampleRate = 16000
// samplesPerTick = 16000/300 = 53.333

// For patterns to specify that there is no note. We have 5 octaves,
// so only notes 0-59 are used, and 63 is the highest that still fits
// in the same number of bits
#define MOD_NO_NOTE 63
// Valid samples are 0-30, use 31 as a blank to fit in 5 bits
#define MOD_NO_SAMPLE 31

#define MOD_DEFAULT_SPEED 6
#define MOD_DEFAULT_TEMPO 125

typedef enum _MOD_STATE {
  MOD_STATE_STOP,
  MOD_STATE_PLAY,
  MOD_STATE_PAUSE,

} MOD_STATE;

typedef struct _SOUND_VARS {
  s8 *mixBufferBase;
  s8 *curMixBuffer;
  u32 mixBufferSize;
  u16 mixFreq;
  u16 rcpMixFreq;
  s16 samplesUntilMODTick;
  u16 samplesPerMODTick;
  u8 activeBuffer;

} SOUND_VARS;

// This is the layout of the sample info in ROM
typedef struct _SAMPLE_HEADER {
  u16 length;
  u8 finetune;
  u8 vol;
  u16 loopStart;
  u16 loopLength;

  const s8 *smpData; // Pointer to sample data in ROM

} SAMPLE_HEADER;

// This is the MOD data layout in ROM
typedef struct _MOD_HEADER {
  const SAMPLE_HEADER *sample;
  const u8 *order;
  const u8 **pattern;

  u8 orderCount;
  u8 pad[3];

} MOD_HEADER;

// This is the data we need to keep track of for each channel
// while playing the song, stored in RAM
typedef struct _MOD_CHANNEL {
  u32 frequency; // Current frequency of note being played, in Hz

  u8 sample; // Last sample used on this channel
  u8 vol;    // Current volume

  u8 pad[2]; // Align to 4 bytes

} MOD_CHANNEL;

// This is all the data for the currently playing song, in RAM
typedef struct _MOD {
  const SAMPLE_HEADER *sample; // Pointer to table of samples in ROM
  const u8 **pattern;          // Pointer to table of pointers to patterns
  const u8 *order;             // Array of pattern numbers to play

  const u8 *rowPtr; // Current position in current pattern, for quick access

  u8 state; // MOD_STATE enum (stopped/playing/paused)

  u8 speed;
  u8 tick; // When this gets to speed, process a new row and reset it to 0

  u8 curRow; // When this gets to 64, move to the next order and reset to 0

  u8 orderCount;
  u8 curOrder; // When this gets to orderCount, stop the song

  u8 tempo; // In BPM (Hz = BPM*2/5)

  u8 pad;

  MOD_CHANNEL channel[4]; // Current state of each channel

} MOD;

extern MOD sndMod;
extern SOUND_VARS sndVars;
extern const MOD_HEADER dModTable[];

// void SndInit(SND_FREQ freq);

void SndMix(u32 samplesToMix);
void SndUpdate();

extern void SndPlayMOD(u32 modIdx);
extern void MODUpdate();

void initDebugDisplay();
void updateDebugNumbers(int pattern, int row, int tempo);

#endif // MODPLAYER_H
