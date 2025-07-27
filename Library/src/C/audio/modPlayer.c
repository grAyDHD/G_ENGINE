#include "audio/modPlayer.h"
#include "audio/audio.h"
#include "audio/mixer.h"
#include "audio/modFreqTable.h"
#include "core/dma.h"

// ----- Constants -----

#define MOD_DEFAULT_SPEED 6
#define MOD_DEFAULT_TEMPO 125

// ----- Structures -----

// ----- Local function prototypes -----

static void modProcessRow();
static void modPlayNote(u32 channelIdx, u32 note, u32 sample, u32 effect,
                        u32 param);
static void modSetTempo(u32 tempo);

// ----- Variables -----

ModMixerChannel modMixerChannel[MOD_MAX_CHANNELS];
ModTiming modTiming;
ModPlayer modPlayer;

// ----- Functions -----

// Call this once at startup
void modInit() {
  AUDIO->ds = DS_MONO_INIT;
  ENABLE_AUDIO;

  for (int i = 0; i < BUFFER_SIZE * 2; i++) {
    mixbuf.bufBase[i] = 0;
  }

  mixbuf.activeBuffer = bufB;
  mixbuf.position = mixbuf.bufBase;

  modTiming.mixFreq = 16000;
  modTiming.rcpMixFreq = (1 << 28) / modTiming.mixFreq;

  // initialize channel structures
  for (int i = 0; i < MOD_MAX_CHANNELS; i++) {
    modMixerChannel[i].data = 0;
    modMixerChannel[i].pos = 0;
    modMixerChannel[i].inc = 0;
    modMixerChannel[i].vol = 0;
    modMixerChannel[i].length = 0;
    modMixerChannel[i].loopLength = 0;
  }

  // start up the timer we will be using
  //{64612, 18157, 304}
  TIMER[0].value = 0xFBE8;
  TIMER[0].control = TMR_ENABLE;

  // set up the DMA settings, but let the VBlank interrupt
  // actually start it up, so the timing is right
  DMA[1].source = (u32)mixbuf.bufB;
  DMA[1].destination = (u32)FIFO_A;
  DMA[1].control = DMA_DEST_FIXED | DMA_REPEAT | DMA_32BIT | DMA_START_SPECIAL |
                   DMA_ENABLE | DMA_IRQ_ENABLE;

} // SndInit

void modUpdate() {
  s32 samplesLeft = BUFFER_SIZE;

  while (samplesLeft > 0) {
    if (modTiming.samplesUntilMODTick == 0 &&
        modPlayer.state == MOD_STATE_PLAY) {
      modAdvance();
      modTiming.samplesUntilMODTick = modTiming.samplesPerMODTick;
    }

    if (modTiming.samplesUntilMODTick < samplesLeft &&
        modPlayer.state == MOD_STATE_PLAY) {
      modMix(modTiming.samplesUntilMODTick);
      samplesLeft -= modTiming.samplesUntilMODTick;

      modTiming.samplesUntilMODTick = 0;
    } else {
      modMix(samplesLeft);
      modTiming.samplesUntilMODTick -= samplesLeft;
      samplesLeft = 0;
    }
  }

} // SndUpdate

// This is only called by SndUpdate
void modMix(u32 samplesToMix) {
  s32 i, ch;

  s16 tempBuffer[BUFFER_SIZE] = {0};

  /*
  i = 0;
  Dma3(tempBuffer, &i, (samplesToMix + 1) * sizeof(s16) / 4, DMA_MEMSET32);
  */

  for (ch = 0; ch < MOD_MAX_CHANNELS; ch++) {
    ModMixerChannel *chnPtr = &modMixerChannel[ch];

    if (chnPtr->data != 0) { // check if channel is active
      for (i = 0; i < samplesToMix; i++) {
        tempBuffer[i] += chnPtr->data[chnPtr->pos >> 12] * chnPtr->vol;
        chnPtr->pos += chnPtr->inc;

        if (chnPtr->pos >= chnPtr->length) {
          if (chnPtr->loopLength == 0) { // if not looping, disable channel
            chnPtr->data = 0;
            i = samplesToMix;
          } else {
            while (chnPtr->pos >= chnPtr->length) {
              chnPtr->pos -= chnPtr->loopLength;
            }
          }
        }
      } // end for i = 0 to bufSize
    } // end data != 0
  } // end channel loop

  for (i = 0; i < samplesToMix; i++) {
    mixbuf.position[i] = tempBuffer[i] >> 8;
  }
  mixbuf.position += samplesToMix;
} // SndMix

void playMod(u32 modIdx) {
  const ModHeader *modHeader = &dModTable[modIdx];

  modPlayer.sample = modHeader->sample;
  modPlayer.pattern = modHeader->pattern;
  modPlayer.order = modHeader->order;
  modPlayer.orderCount = modHeader->orderCount;
  modPlayer.curOrder = 0;
  modPlayer.curRow = 0;
  modPlayer.tick = 0;
  modPlayer.speed = MOD_DEFAULT_SPEED;
  modPlayer.rowPtr = modPlayer.pattern[modPlayer.order[0]];
  modPlayer.state = MOD_STATE_PLAY;

  // Set to default
  modSetTempo(MOD_DEFAULT_TEMPO);
  modTiming.samplesUntilMODTick = 0;

} // SndPlayMOD

void modAdvance() {
  if (++modPlayer.tick >= modPlayer.speed) {
    modPlayer.tick = 0;

    if (++modPlayer.curRow >= 64) {
      modPlayer.curRow = 0;

      if (++modPlayer.curOrder >= modPlayer.orderCount) {
        modPlayer.state = MOD_STATE_STOP;
        return;
      } else {
        modPlayer.rowPtr =
            modPlayer.pattern[modPlayer.order[modPlayer.curOrder]];
      }
    }

    modProcessRow();
  } else {
    // We'll update the non row-tick effects here later
  }

} // updateMod

static void modProcessRow() {
  s32 curChannel;

  for (curChannel = 0; curChannel < MOD_MAX_CHANNELS; curChannel++) {
    u8 note, sample, effect, param;

    note = *modPlayer.rowPtr++;
    sample = *modPlayer.rowPtr++;
    effect = *modPlayer.rowPtr++;
    param = *modPlayer.rowPtr++;

    if (sample == MOD_NO_SAMPLE) {
      sample = modPlayer.channel[curChannel].sample;
    } else {
      modPlayer.channel[curChannel].sample = sample;
      // Another tricky thing to know about MOD: Volume is only set when
      // specifying new samples, NOT when playing notes, and it is set even
      // when there is a sample, but no note specified (although the sample
      // playing doesn't change in that case)
      modPlayer.channel[curChannel].vol = modPlayer.sample[sample].vol;
      modMixerChannel[curChannel].vol = modPlayer.channel[curChannel].vol;
    }

    if (note != MOD_NO_NOTE) {
      modPlayNote(curChannel, note, sample, effect, param);
    }
  }

} // MODProcessRow

static void modPlayNote(u32 channelIdx, u32 note, u32 sampleIdx, u32 effect,
                        u32 param) {
  ModMixerChannel *sndChn;
  ModChannel *modChn;
  const SampleHeader *sample;
  u8 finetune;

  // Here's that special case that they didn't specify a sample before playing a
  // note
  if (sampleIdx == MOD_NO_SAMPLE) {
    return;
  }

  // These make things less cumbersome
  sndChn = &modMixerChannel[channelIdx];
  modChn = &modPlayer.channel[channelIdx];
  sample = &modPlayer.sample[sampleIdx];

  // 60 notes total, and one full set of notes for each finetune level
  modChn->frequency = modFreqTable[sample->finetune * 60 + note];

  // Set up the mixer channel
  sndChn->data = sample->smpData;
  sndChn->pos = 0;
  sndChn->inc =
      modChn->frequency * modTiming.rcpMixFreq >> 16; // Explained below

  // Next member is volume, but skip setting it because it doesn't change unless
  // a new sample was specified, in which case it was already set back in
  // MODProcessRow

  // If looping, use loopStart+loopLength as the ending point,
  // otherwise just use normal length.
  // Length and loop length are still half what the real size is
  // (to fit in 16 bits, as per MOD format), so they need to be
  // shifted left 1. They also need to be shifted left 12 because
  // the mixer compares the 12-bit fixed-point position against
  // them directly, so that comes to a total shift left of 13
  sndChn->length =
      (sample->loopLength != 0 ? sample->loopStart + sample->loopLength
                               : sample->length)
      << 13;
  sndChn->loopLength = sample->loopLength << 13;

} // MODPlayNote

static void modSetTempo(u32 tempo) {
  u32 modFreq;

  modPlayer.tempo = tempo;
  modFreq = (tempo * 2) / 5;

  modTiming.samplesUntilMODTick -= modTiming.samplesPerMODTick;
  modTiming.samplesPerMODTick = modTiming.mixFreq / modFreq;
  modTiming.samplesUntilMODTick += modTiming.samplesPerMODTick;
} // MODSetTempo
