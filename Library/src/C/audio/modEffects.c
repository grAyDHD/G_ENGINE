#include "audio/modEffects.h"

// Move just the speed effect here for now
void MODFXSpeed(ModEffectUpdateData *data) {
  if (data->param < 32) {
    modPlayer.speed = data->param;
  } else {
    modSetTempo(data->param); // This will cause a compilation error for now
  }
}

const ModEffect modEffect[MOD_EFFECT_TIMING_COUNT][16] = {
    {
        // MOD_EFFECT_TABLE_ROW
        NULL,      // 0x0: Arpeggio
        NULL,      // 0x1: Porta up
        NULL,      // 0x2: Porta down
        NULL,      // 0x3: Tone porta
        NULL,      // 0x4: Vibrato
        NULL,      // 0x5: Volslide+Tone porta
        NULL,      // 0x6: Volslide+Vibrato
        NULL,      // 0x7: Tremolo
        NULL,      // 0x8: Set panning
        NULL,      // 0x9: Sample offset
        NULL,      // 0xA: Volume slide
        NULL,      // 0xB: Jump to order
        NULL,      // 0xC: Set volume
        NULL,      // 0xD: Break to row
        NULL,      // 0xE: Special (more on this later)
        MODFXSpeed // 0xF: Speed/Tempo
    },
    {
        // MOD_EFFECT_TABLE_MID
        NULL, // 0x0: Arpeggio
        NULL, // 0x1: Porta up
        NULL, // 0x2: Porta down
        NULL, // 0x3: Tone porta
        NULL, // 0x4: Vibrato
        NULL, // 0x5: Volslide+Tone porta
        NULL, // 0x6: Volslide+Vibrato
        NULL, // 0x7: Tremolo
        NULL, // 0x8: Set panning
        NULL, // 0x9: Sample offset
        NULL, // 0xA: Volume slide
        NULL, // 0xB: Jump to order
        NULL, // 0xC: Set volume
        NULL, // 0xD: Break to row
        NULL, // 0xE: Special (more on this later)
        NULL  // 0xF: Speed/Tempo
    },
};

const ModEffectUpdateData modDefaultData[MOD_EFFECT_TIMING_COUNT] = {
    {
        // MOD_EFFECT_TIMING_ROW
        NULL, NULL,                       // modCh, mixCh (filled in later)
        MOD_NO_NOTE, MOD_NO_SAMPLE, 0, 0, // note, sample, effect, param
        MOD_PLAY_NOTE // Default: try to play notes on row-ticks
    },
    {
        // MOD_EFFECT_TIMING_MID
        NULL, NULL,                       // modCh, mixCh (filled in later)
        MOD_NO_NOTE, MOD_NO_SAMPLE, 0, 0, // note, sample, effect, param
        0 // Default: don't do anything on mid-ticks
    },
};

void modHandleUpdateFlags(ModEffectUpdateData *data) {

  if ((data->note != MOD_NO_NOTE) && (data->updateFlags & MOD_PLAY_NOTE)) {
    modPlayNote(data);
  }

  if (data->updateFlags & MOD_SET_VOL) {
    data->mixCh->vol = data->modCh->vol;
  }

  if (data->updateFlags & MOD_SET_FREQ) {
    data->mixCh->inc = data->modCh->frequency * modTiming.rcpMixFreq >> 16;
  }

} // modHandleUpdateFlags

void modUpdateEffects() {
  s32 ch;

  for (ch = 0; ch < MOD_MAX_CHANNELS; ch++) {
    // Bail if there's no effect to update
    if (modPlayer.channel[ch].effect != 0 || modPlayer.channel[ch].param != 0) {
      // Initialize with mid-tick values now
      ModEffectUpdateData data = modDefaultData[MOD_EFFECT_TIMING_MID];

      data.modCh = &modPlayer.channel[ch];
      data.mixCh = &modMixerChannel[ch];

      // Already made sure there was an effect, so just check the function.
      // Notice that we're using the mid-tick table now.
      if (modEffect[MOD_EFFECT_TIMING_MID][data.modCh->effect] != NULL)
        modEffect[MOD_EFFECT_TIMING_MID][data.modCh->effect](&data);

      modHandleUpdateFlags(&data);
    }
  }
} // modUpdateEffects
