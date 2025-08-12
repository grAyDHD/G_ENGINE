#include "audio/modPlayer.h"
#include "audio/audio.h"
#include "audio/mixer.h"
#include "audio/modEffects.h"
#include "audio/modFreqTable.h"
#include "core/dma.h"

// ----- Constants -----

#define MOD_DEFAULT_SPEED 6
#define MOD_DEFAULT_TEMPO 125

// ----- Structures -----

// ----- Local function prototypes -----
static void modProcessRow();

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
  modTiming.mixFreqPeriod = ((AMIGA_VAL << 9) / modTiming.mixFreq) << 3;

  // initialize channel structures
  for (int i = 0; i < MOD_MAX_CHANNELS; i++) {
    modMixerChannel[i].data = 0;
    modMixerChannel[i].position = 0;
    modMixerChannel[i].increment = 0;
    modMixerChannel[i].volume = 0;
    modMixerChannel[i].length = 0;
    modMixerChannel[i].loopLength = 0;
  }

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
    if (modTiming.samplesUntilModTick < (1 << 12) &&
        modPlayer.state == MOD_STATE_PLAY) {
      modAdvance();
      modTiming.samplesUntilModTick += modTiming.samplesPerModTick;
    }

    if ((modTiming.samplesUntilModTick >> 12) < samplesLeft &&
        modPlayer.state == MOD_STATE_PLAY) {
      modMix(modTiming.samplesUntilModTick >> 12);
      samplesLeft -= modTiming.samplesUntilModTick >> 12;

      modTiming.samplesUntilModTick -= (modTiming.samplesUntilModTick >> 12)
                                       << 12;
    } else {
      modMix(samplesLeft);
      modTiming.samplesUntilModTick -= samplesLeft << 12;
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
        tempBuffer[i] += chnPtr->data[chnPtr->position >> 12] * chnPtr->volume;
        chnPtr->position += chnPtr->increment;

        if (chnPtr->position >= chnPtr->length) {
          if (chnPtr->loopLength == 0) { // if not looping, disable channel
            chnPtr->data = 0;
            i = samplesToMix;
          } else {
            while (chnPtr->position >= chnPtr->length) {
              chnPtr->position -= chnPtr->loopLength;
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
  modTiming.samplesUntilModTick = 0;

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
    modUpdateEffects();
  }

} // updateMod

static void modProcessRow() {
  s32 ch;

  for (ch = 0; ch < MOD_MAX_CHANNELS; ch++) {
    ModEffectUpdateData data = modDefaultData[MOD_EFFECT_TIMING_ROW];

    data.modCh = &modPlayer.channel[ch];
    data.mixCh = &modMixerChannel[ch];

    data.note = *modPlayer.rowPtr++;
    data.sample = *modPlayer.rowPtr++;
    data.effect = *modPlayer.rowPtr++;
    data.param = *modPlayer.rowPtr++;

    // Set these for the mid-ticks
    data.modCh->note = data.note;
    data.modCh->effect = data.effect;
    data.modCh->param = data.param;

    if (data.sample != MOD_NO_SAMPLE) { // Never set local to memory anymore
      data.modCh->sample = data.sample;
      data.modCh->volume = modPlayer.sample[data.sample].vol;

      // Don't set mixer channel volume until after effect processing
      // vars.sndChn->vol          = vars.modChn->vol;
      data.updateFlags |= MOD_SET_VOL;
    }

    if ((data.effect != 0 || data.param != 0) &&
        (modEffect[MOD_EFFECT_TIMING_ROW][data.effect] != NULL)) {
      modEffect[MOD_EFFECT_TIMING_ROW][data.effect](&data);
    }

    modHandleUpdateFlags(&data);
  }
} // MODProcessRow

void modPlayNote(ModEffectUpdateData *data) {
  ModMixerChannel *sndChn;
  ModChannel *modChn;
  const SampleHeader *sample;
  u8 finetune;

  // Here's that special case that they didn't specify a sample before playing
  // a note
  if (data->sample == MOD_NO_SAMPLE) {
    return;
  }

  sample = &modPlayer.sample[data->modCh->sample];
  data->modCh->period = modPeriodTable[data->modCh->finetune * 60 + data->note];

  // set up mixer channel
  data->mixCh->data = sample->smpData;
  data->mixCh->position = data->sampleOffset << 20;

  // Let update flags take care of setting the inc
  // because it may also need to be set by effects without playing a note
  // vars->sndChn->inc        = vars->modChn->frequency * sndVars.rcpMixFreq >>
  // 16;
  data->updateFlags |= MOD_SET_FREQ;

  data->mixCh->length =
      (sample->loopLength != 0 ? sample->loopStart + sample->loopLength
                               : sample->length)
      << 13;
  data->mixCh->loopLength = sample->loopLength << 13;
} // modPlayNote

void modSetTempo(u32 tempo) {
  // u32 modFreq;

  // modPlayer.tempo = tempo;
  // modFreq = (tempo * 2) / 5;
  modTiming.samplesPerModTick = (modTiming.mixFreq * 5 << 12) / (tempo * 2);

  // modTiming.samplesUntilMODTick -= modTiming.samplesPerMODTick;
  // modTiming.samplesPerMODTick = modTiming.mixFreq / modFreq;
  // modTiming.samplesUntilMODTick += modTiming.samplesPerMODTick;
} // modSetTempo
