#include "audio/modEffects.h"
#include "audio/modFreqTable.h"
#include "audio/modPlayer.h"

void modFxArpeggioRow(ModEffectUpdateData *data) {
  data->modCh->arpeggioTick = 0;
  if (data->modCh->note == MOD_NO_NOTE) {
    data->modCh->effect = data->modCh->param = 0;
  }
}

void modFxArpeggioMid(ModEffectUpdateData *data) {
  u32 arpNote;

  if (++data->modCh->arpeggioTick > 2) {
    data->modCh->arpeggioTick = 0;
  }

  switch (data->modCh->arpeggioTick) {
  case 0:
    arpNote = data->modCh->note;
    break;
  case 1:
    arpNote = data->modCh->note + (data->modCh->param >> 4);
    break;
  case 2:
    arpNote = data->modCh->note + (data->modCh->param & 0xF);
    break;
  }

  if (arpNote > MOD_HIGHEST_NOTE) {
    arpNote = MOD_HIGHEST_NOTE;
  }

  data->modCh->period = modPeriodTable[data->modCh->finetune * 60 + arpNote];
  data->updateFlags |= MOD_SET_FREQ;
} // modFxArpeggioMid

static s32 modPitchSlide(s32 period, s32 slide) {
  period += slide;
  if (period > MOD_PERIOD_MAX) {
    period = MOD_PERIOD_MAX;
  } else if (period < MOD_PERIOD_MIN) {
    period = MOD_PERIOD_MIN;
  }

  return period;
}

static void modFxPortamentoRow(ModEffectUpdateData *data) {
  if (data->param != 0) {
    data->modCh->portaSpeed = data->param;
  }
}

static void modFxPortamentoUp(ModEffectUpdateData *data) {
  data->modCh->period =
      modPitchSlide(data->modCh->period,
                    -data->modCh->portaSpeed); // negative = higher pitch
  data->updateFlags |= MOD_SET_FREQ;
}

static void modFxPortamentoDown(ModEffectUpdateData *data) {
  data->modCh->period =
      modPitchSlide(data->modCh->period,
                    data->modCh->portaSpeed); // positive = lower pitch
  data->updateFlags |= MOD_SET_FREQ;
}

void modFxTonePortamentoRow(ModEffectUpdateData *data) {
  if (data->note != MOD_NO_NOTE) {
    data->modCh->tonePortaTarget = data->note;
  }

  if (data->param != 0) {
    data->modCh->tonePortaSpeed = data->param;
  }

  data->updateFlags &= MOD_PLAY_NOTE;
}

void modFxTonePortamentoMid(ModEffectUpdateData *data) {
  u16 targetPeriod =
      modPeriodTable[data->modCh->finetune * 60 + data->modCh->tonePortaTarget];

  if (data->modCh->period < targetPeriod) {
    data->modCh->period =
        modPitchSlide(data->modCh->period, data->modCh->tonePortaSpeed);
    if (data->modCh->period > targetPeriod) {
      data->modCh->period = targetPeriod;
    }
  } else if (data->modCh->period > targetPeriod) {
    data->modCh->period =
        modPitchSlide(data->modCh->period, -data->modCh->tonePortaSpeed);
    if (data->modCh->period < targetPeriod) {
      data->modCh->period = targetPeriod;
    }
  }
}

void modFxVibrato(ModEffectUpdateData *data) {}

void modFxVolumeSlideTonePortaRow(ModEffectUpdateData *data) {
  modFxVolumeSlideRow(data);
  data->param = 0;

  modFxTonePortamentoRow(data);
}

void modFxVolumeSlideTonePortaMid(ModEffectUpdateData *data) {
  modFxVolumeSlideMid(data);
  modFxTonePortamentoMid(data);
}

void modFxVolumeSlideVibrato(ModEffectUpdateData *data) {}

void modFxTremolo(ModEffectUpdateData *data) {}

void modFxSetPanning(ModEffectUpdateData *data) {}

void modFxSampleOffset(ModEffectUpdateData *data) {
  data->sampleOffset = data->param;
}

static u32 modFxVolumeSlide(u32 volume, s32 slide) {
  volume += slide;
  if (volume > 64) {
    if (slide > 0) {
      volume = 64;
    } else {
      volume = 0;
    }
  }
  return volume;
}

void modFxVolumeSlideRow(ModEffectUpdateData *data) {
  if (data->param != 0) {
    if ((data->param & 0x0F) == 0) { // crescendo
      data->modCh->volumeSlideSpeed = data->param >> 4;
    } else if (data->param & 0xF0) {
      data->modCh->volumeSlideSpeed = -data->param;
    } else {
      data->modCh->effect = data->modCh->param = 0;
    }
  }
}

void modFxVolumeSlideMid(ModEffectUpdateData *data) {
  data->modCh->volume =
      modFxVolumeSlide(data->modCh->volume, data->modCh->volumeSlideSpeed);
  data->updateFlags |= MOD_SET_VOL;
}

void modFxJumpToOrder(ModEffectUpdateData *data) {}

void modFxSetVolume(ModEffectUpdateData *data) {
  data->modCh->volume = data->param;
  if (data->modCh->volume > 64) {
    data->modCh->volume = 64;
  }
}

void modFxBreakToRow(ModEffectUpdateData *data) {}
/*
 * callback format:
 * bRowTick is actually a bool, transition to enum
 * void function(u32 param, int bRowTick) {
 *   if (bRowTick == 1) {
 *     doThing(param);
 *   }
 * }
 * */

void modFxSpecialRow(ModEffectUpdateData *data) {
  u32 param = data->modCh->param & 0xF;

  // half of modCh->param is actual parameter (high 4 bits)
  // low 4 bits represent special effect called in switch
  switch (data->modCh->param >> 4) {
  case 0x0:
    if (modPlayer.callback != NULL) {
      modPlayer.callback(param, 1);
    }
    break;

  case 0x1:
    break;

  case 0x2:
    break;

  case 0x3:
    break;

  case 0x4:
    break;

  case 0x5: // finetune
    data->modCh->finetune = param;
    if (data->modCh->note != MOD_NO_NOTE) {
      data->modCh->period =
          modPeriodTable[data->modCh->finetune * 60 + data->modCh->note];
      data->updateFlags |= MOD_SET_FREQ;
    }
    break;

  case 0x6:
    break;

  case 0x7:
    break;

  case 0x8:
    break;

  case 0x9:
    break;

  case 0xA:
    break;

  case 0xB:
    break;

  case 0xC:
    break;

  case 0xD:
    break;

  case 0xE:
    break;

  case 0xF:
    break;
  }
}

void modFxSpecialMid(ModEffectUpdateData *data) {
  u32 param = data->modCh->param & 0xF;

  switch (data->modCh->param >> 4) {
  case 0x0: // callback
    if (modPlayer.callback != NULL) {
      modPlayer.callback(param, 0);
    }
    break;

  case 0x1:
    break;

  case 0x2:
    break;

  case 0x3:
    break;

  case 0x4:
    break;

  case 0x5:
    break;

  case 0x6:
    break;

  case 0x7:
    break;

  case 0x8:
    break;

  case 0x9:
    break;

  case 0xA:
    break;

  case 0xB:
    break;

  case 0xC:
    break;

  case 0xD:
    break;

  case 0xE:
    break;

  case 0xF:
    break;
  }
}

void modFxSetCallback(ModCallback callback) { modPlayer.callback = callback; }

void modFxSetSpeed(ModEffectUpdateData *data) {
  if (data->param < 32) {
    modPlayer.speed = data->param;
  }
}
/*

// Save ourselves the trouble of initializing each member in code
static const MOD_UPDATE_VARS modDefaultVars[MOD_EFFECT_TABLE_NUM] = {
    {
        // MOD_EFFECT_TABLE_ROW
        NULL,                  // modChn
        NULL,                  // sndChn
        MOD_NO_NOTE,           // note
        MOD_NO_SAMPLE,         // sample
        0,                     // effect
        0,                     // param
        MOD_UPD_FLG_PLAY_NOTE, // updateFlags
        0,                     // fineSlide
        0,                     // smpOffset
    },
    {
        // MOD_EFFECT_TABLE_MID
        NULL,          // modChn
        NULL,          // sndChn
        MOD_NO_NOTE,   // note
        MOD_NO_SAMPLE, // sample
        0,             // effect
        0,             // param
        0,             // updateFlags
        0,             // fineSlide
        0,             // smpOffset
    }};

static const EFFECT_FUNC_PTR modEffectTable[MOD_EFFECT_TABLE_NUM][16] = {
    {
        // Row-tick updates
        MODFXArpeggioRow,    // 0x0: Arpeggio
        MODFXPortaRow,       // 0x1: Porta up
        MODFXPortaRow,       // 0x2: Porta down
        MODFXTonePortaRow,   // 0x3: Tone porta
        MODFXVibratoRow,     // 0x4: Vibrato
        MODFXVSldTPortaRow,  // 0x5: Volslide+Tone porta
        MODFXVSldVibratoRow, // 0x6: Volslide+Vibrato
        MODFXTremoloRow,     // 0x7: Tremolo
        NULL,                // 0x8: Set panning (unsupported)
        MODFXSampleOffset,   // 0x9: Sample offset
        MODFXVolslideRow,    // 0xA: Volume slide
        MODFXJumpToOrder,    // 0xB: Jump to order
        MODFXSetVol,         // 0xC: Set volume
        MODFXBreakToRow,     // 0xD: Break to row
        MODFXSpecialRow,     // 0xE: Special
        MODFXSpeed           // 0xF: Speed/Tempo
    },
    {
        // Non row-tick updates
        MODFXArpeggioMid,    // 0x0: Arpeggio
        MODFXPortaUpMid,     // 0x1: Porta up
        MODFXPortaDownMid,   // 0x2: Porta down
        MODFXTonePortaMid,   // 0x3: Tone porta
        MODFXVibratoMid,     // 0x4: Vibrato
        MODFXVSldTPortaMid,  // 0x5: VolSlide+Tone porta
        MODFXVSldVibratoMid, // 0x6: VolSlide+Vibrato
        MODFXTremoloMid,     // 0x7: Tremolo
        NULL,                // 0x8: Set panning
        NULL,                // 0x9: Sample offset
        MODFXVolslideMid,    // 0xA: Volume slide
        NULL,                // 0xB: Jump to order
        NULL,                // 0xC: Set volume
        NULL,                // 0xD: Break to row
        MODFXSpecialMid,     // 0xE: Special
        NULL                 // 0xF: Speed/Tempo
    }};


 */

const ModEffect modEffect[MOD_EFFECT_TIMING_COUNT][16] = {
    {
        // MOD_EFFECT_TABLE_ROW
        modFxArpeggioRow,    // 0x0: Arpeggio
        modFxPortamentoRow,  // 0x1: Porta up
        modFxPortamentoRow,  // 0x2: Porta down
        NULL,                // 0x3: Tone porta
        NULL,                // 0x4: Vibrato
        NULL,                // 0x5: Volslide+Tone porta
        NULL,                // 0x6: Volslide+Vibrato
        NULL,                // 0x7: Tremolo
        NULL,                // 0x8: Set panning
        NULL,                // 0x9: Sample offset
        modFxVolumeSlideRow, // 0xA: Volume slide
        NULL,                // 0xB: Jump to order
        modFxSetVolume,      // 0xC: Set volume
        NULL,                // 0xD: Break to row
        NULL,                // 0xE: Special (more on this later)
        modFxSetSpeed,       // 0xF: Speed/Tempo
    },
    {
        // MOD_EFFECT_TABLE_MID
        modFxArpeggioMid,    // 0x0: Arpeggio
        modFxPortamentoUp,   // 0x1: Porta up
        modFxPortamentoDown, // 0x2: Porta down
        NULL,                // 0x3: Tone porta
        NULL,                // 0x4: Vibrato
        NULL,                // 0x5: Volslide+Tone porta
        NULL,                // 0x6: Volslide+Vibrato
        NULL,                // 0x7: Tremolo
        NULL,                // 0x8: Set panning
        modFxSampleOffset,   // 0x9: Sample offset
        modFxVolumeSlideMid, // 0xA: Volume slide
        NULL,                // 0xB: Jump to order
        NULL,                // 0xC: Set volume
        NULL,                // 0xD: Break to row
        NULL,                // 0xE: Special (more on this later)
        NULL                 // 0xF: Speed/Tempo
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
        0, // Default: don't do anything on mid-ticks
        0, // Default: no offset
    },
};

void modHandleUpdateFlags(ModEffectUpdateData *data) {

  if ((data->note != MOD_NO_NOTE) && (data->updateFlags & MOD_PLAY_NOTE)) {
    modPlayNote(data);
  }

  if (data->updateFlags & MOD_SET_VOL) {
    data->mixCh->volume = data->modCh->volume;
  }

  if (data->updateFlags & MOD_SET_FREQ) {
    data->mixCh->increment = modTiming.mixFreqPeriod / data->modCh->period;
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
