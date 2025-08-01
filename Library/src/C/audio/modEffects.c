#include "audio/modEffects.h"
#include "audio/modPlayer.h"

/*
// Move just the speed effect here for now
void MODFXSpeed(ModEffectUpdateData *data) {
  if (data->param < 32) {
    modPlayer.speed = data->param;
  } else {
    modSetTempo(data->param); // This will cause a compilation error for now
  }
}
*/

// --- Effect helper functions ---

// Returns volume after sliding up/down and clamping to (0,64)
static u32 MODVolumeSlide(u32 volume, s32 slide) {
  volume += slide;
  if (volume > 64) // Since volume is unsigned, this covers
  {                // sliding too far in either direction
    if (slide > 0)
      volume = 64; // Must have gone above 64
    else
      volume = 0; // Must have gone below 0 and wrapped
  }

  return volume;
}

/*
// Returns period after sliding up/down and
// clamping to (MOD_PERIOD_MIN,MOD_PERRIOD_MAX)
static s32 MODPitchSlide(s32 period, s32 slide) {
  period += slide;
  if (period > MOD_PERIOD_MAX)
    period = MOD_PERIOD_MAX;
  else if (period < MOD_PERIOD_MIN)
    period = MOD_PERIOD_MIN;

  return period;
}
*/

/*
static void MODInitVibrato(MOD_UPDATE_VARS *vars, MOD_VIBRATO_PARAMS *vibrato) {
  if (vars->param & 0xf != 0)
    vibrato->depth = vars->param & 0xf;
  if (vars->param >> 4 != 0)
    vibrato->speed = vars->param >> 4;
  if (vibrato->noRetrig == FALSE && vars->note != MOD_NO_NOTE)
    vibrato->tick = 0;
}
*/

/*
// Updates vibrato/tremolo, setting vibrato->slide to the range (-64, 63)
static void MODUpdateVibrato(MOD_VIBRATO_PARAMS *vibrato) {
  // Increment the tick. All wave types use a cycle of 0-63 on
  // the tick. Since it's a 6-bit bitfield, it wraps automatically.
  vibrato->tick += vibrato->speed;
  // vibrato->tick &= 63;

  switch (vibrato->waveform) {
  case MOD_WAVEFORM_SINE:
    // Sine table is 0-255, and depth is 0-15. Shift 6 places off,
    // giving a range of 0-64 (actually slightly less), and add or
    // subtract it to get the final range +-64
    vibrato->slide = vibratoSineTab[vibrato->tick] * vibrato->depth >> 6;
    if (vibrato->tick >= 32)            // First 32 ticks are the
      vibrato->slide = -vibrato->slide; // positive half of the sine
    break;                              // wave, next 32 are negative

  case MOD_WAVEFORM_RAMP:
    // This is a linear ramp down, 64 ticks long. Subtract tick from
    // 32 to give a value of 32 at tick 0, and -31 at tick 63 (just
    // before wrapping), effectively creating a downward slide.
    // Divide by 8 for final range of +-64
    vibrato->slide = (32 - vibrato->tick) * vibrato->depth >> 3;
    break;

  case MOD_WAVEFORM_SQUARE:
    // Square wave. Alternate between higest and lowest values.
    // Since the final range needs to be +-64 and the depth ranges
    // 0-15, just add or subtract 4x the depth
    vibrato->slide = vibrato->depth << 2;
    if (vibrato->tick >= 32)
      vibrato->slide = -vibrato->slide;
    break;

  case MOD_WAVEFORM_RANDOM:
    // Since this table only ranges -128 to +127, shift down
    // one less than sine to give the same final range
    vibrato->slide = vibratoRandomTab[vibrato->tick] * vibrato->depth >> 5;
    break;
  }

} // MODUpdateVibrato
*/

// --- MOD effect functions ---

static void MODFXSetVol(ModEffectUpdateData *data) {
  data->modCh->vol = data->param;
  if (data->modCh->vol > 64)
    data->modCh->vol = 64;
  data->updateFlags |= MOD_SET_VOL;
}

static void MODFXSpeed(ModEffectUpdateData *vars) {
  if (vars->param < 32) // 0-31 = set speed
    modPlayer.speed = vars->param;
  else // 32-255 = set tempo
    modSetTempo(vars->param);
}

/*
static void MODFXSampleOffset(ModEffectUpdateData *vars) {
  vars->smpOffset = vars->param;
}
*/

/*
static void MODFXJumpToOrder(ModEffectUpdateData *vars) {
  modPlayer.nextOrder = vars->param;
  modPlayer.curRow = MOD_PATTERN_ROWS; // Break next update
}
*/

/*
static void MODFXBreakToRow(MOD_UPDATE_VARS *vars) {
  sndMod.breakRow = vars->param;
  sndMod.curRow = MOD_PATTERN_ROWS; // Break next update
}
*/

/*
static void MODFXArpeggioRow(ModEffectUpdateData *vars) {
  vars->modCh->arpeggioTick = 0;
  // Cancel the effect if no note has been played on this channel
  if (vars->note == MOD_NO_NOTE)
    vars->modCh->effect = vars->modChn->param = 0;
}
*/

/*
static void MODFXArpeggioMid(MOD_UPDATE_VARS *vars) {
  u32 arpNote;

  if (++vars->modChn->arpeggioTick > 2)
    vars->modChn->arpeggioTick = 0;

  switch (vars->modChn->arpeggioTick) {
  case 0:
    arpNote = vars->modChn->note;
    break;
  case 1:
    arpNote = vars->modChn->note + (vars->modChn->param >> 4);
    break;
  case 2:
    arpNote = vars->modChn->note + (vars->modChn->param & 0xf);
    break;
  }

  if (arpNote > MOD_MAX_NOTE)
    arpNote = MOD_MAX_NOTE;

  vars->modChn->period = notePeriodTable[vars->modChn->finetune * 60 + arpNote];

  vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;

} // MODFXArpeggioMid
*/

/*
static void MODFXVolslideRow(MOD_UPDATE_VARS *vars) {
  if (vars->param != 0) {
    if ((vars->param & 0x0f) == 0)
      vars->modChn->volslideSpeed = vars->param >> 4;
    else if ((vars->param & 0xf0) == 0)
      vars->modChn->volslideSpeed = -vars->param;
    else
      vars->modChn->effect = vars->modChn->param = 0;
  }
}
*/

/*
static void MODFXVolslideMid(MOD_UPDATE_VARS *vars) {
  vars->modChn->vol =
      MODVolumeSlide(vars->modChn->vol, vars->modChn->volslideSpeed);
  vars->updateFlags |= MOD_UPD_FLG_SET_VOL;
}
*/

/*
static void MODFXPortaRow(MOD_UPDATE_VARS *vars) {
  if (vars->param != 0)
    vars->modChn->portaSpeed = vars->param;
}
*/

/*
static void MODFXPortaDownMid(MOD_UPDATE_VARS *vars) {
  vars->modChn->period =
      MODPitchSlide(vars->modChn->period, vars->modChn->portaSpeed);

  vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
}

static void MODFXPortaUpMid(MOD_UPDATE_VARS *vars) {
  vars->modChn->period =
      MODPitchSlide(vars->modChn->period, -vars->modChn->portaSpeed);

  vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
}
*/

/*
static void MODFXTonePortaRow(MOD_UPDATE_VARS *vars) {
  if (vars->modChn->sample != MOD_NO_SAMPLE) {
    if (vars->note != MOD_NO_NOTE)
      vars->modChn->tonePortaNote = vars->note;
    // Else use the note like it is

    if (vars->param != 0)
      vars->modChn->tonePortaSpeed = vars->param;
    // Else use the speed like it is

    // Don't play the note or we wouldn't have anything to slide to
    vars->updateFlags &= ~MOD_UPD_FLG_PLAY_NOTE;
  } else // No sample. Can't read finetune on mid-ticks, so cancel the effect
  {
    vars->modChn->effect = vars->modChn->param = 0;
  }
}

static void MODFXTonePortaMid(MOD_UPDATE_VARS *vars) {
  u32 tonePortaDestFreq;

  // Fetch the frequency of the target note
  tonePortaDestFreq = notePeriodTable[vars->modChn->finetune * 60 +
                                      vars->modChn->tonePortaNote];

  if (vars->modChn->period < tonePortaDestFreq) {
    // Slide down toward note
    vars->modChn->period =
        MODPitchSlide(vars->modChn->period, vars->modChn->tonePortaSpeed);
    if (vars->modChn->period > tonePortaDestFreq) {
      // Slid too far, back up to the destination
      vars->modChn->period = tonePortaDestFreq;
    }
  } else if (vars->modChn->period > tonePortaDestFreq) {
    // Slide up toward note
    vars->modChn->period =
        MODPitchSlide(vars->modChn->period, -vars->modChn->tonePortaSpeed);
    if (vars->modChn->period < tonePortaDestFreq) {
      // Slid too far, back down to the destination
      vars->modChn->period = tonePortaDestFreq;
    }
  }
  // Else we're already at the target frequency

  // Mix channel increment will need recalculated
  vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;

} // MODFXTonePortaMid
*/

/*
static void MODFXVibratoRow(MOD_UPDATE_VARS *vars) {
  MODInitVibrato(vars, &vars->modChn->vibrato);
}

static void MODFXVibratoMid(MOD_UPDATE_VARS *vars) {
  MODUpdateVibrato(&vars->modChn->vibrato);
  vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
}

static void MODFXTremoloRow(MOD_UPDATE_VARS *vars) {
  MODInitVibrato(vars, &vars->modChn->tremolo);
}

static void MODFXTremoloMid(MOD_UPDATE_VARS *vars) {
  MODUpdateVibrato(&vars->modChn->tremolo);
  vars->updateFlags |= MOD_UPD_FLG_SET_VOL;
}

static void MODFXVSldTPortaRow(MOD_UPDATE_VARS *vars) {
  // Param goes with the volslide part, tone porta just continues
  // So handle volslide like normal
  MODFXVolslideRow(vars);
  // Now trick the tone porta into thinking there was a
  // 0 'continue' param. This local param won't be used again,
  // so changing it won't hurt anything
  vars->param = 0;
  MODFXTonePortaRow(vars);
}

static void MODFXVSldTPortaMid(MOD_UPDATE_VARS *vars) {
  MODFXVolslideMid(vars);
  MODFXTonePortaMid(vars);
}

static void MODFXVSldVibratoRow(MOD_UPDATE_VARS *vars) {
  // Param goes with the volslide part, tone porta just continues
  // So handle volslide like normal
  MODFXVolslideRow(vars);
  // Now trick the vibrato into thinking there was a
  // 0 'continue' param. This local param won't be used again,
  // so changing it won't hurt anything
  vars->param = 0;
  MODFXVibratoRow(vars);
}

static void MODFXVSldVibratoMid(MOD_UPDATE_VARS *vars) {
  MODFXVolslideMid(vars);
  MODFXVibratoMid(vars);
}

static void MODFXSpecialRow(MOD_UPDATE_VARS *vars) {
  // Since the upper 4 bits of the parameter are used as the effect
  // type, this saves us having to extract the lower bits every time
  u32 param = vars->modChn->param & 0xf;

  switch (vars->modChn->param >> 4) {
  case 0x0: // Undefined, we use it for callback
    if (sndMod.callback != NULL)
      sndMod.callback(param, TRUE);
    break;
  case 0x1: // Fine porta up
    vars->fineSlide = param;
    vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
    break;
  case 0x2: // Fine porta down
    vars->fineSlide = -param;
    vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
    break;
  case 0x3: // Glissando (unsupported)
    break;
  case 0x4: // Vibrato waveform
    vars->modChn->vibrato.waveform = param & 3;
    vars->modChn->vibrato.noRetrig = (param & 4) ? TRUE : FALSE;
    break;
  case 0x5: // Set finetune
    vars->modChn->finetune = param;
    if (vars->modChn->note != MOD_NO_NOTE) {
      vars->modChn->period =
          notePeriodTable[vars->modChn->finetune * 60 + vars->modChn->note];
      vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
    }
    break;
  case 0x6: // Pattern loop
    if (param == 0)
      vars->modChn->patLoopPos = sndMod.curRow;
    else {
      if (vars->modChn->patLoopCount == 0)
        vars->modChn->patLoopCount = param + 1;

      if (--vars->modChn->patLoopCount !=
          0) { // Loop back to the stored row in this order next tick
        sndMod.breakRow = vars->modChn->patLoopPos;
        sndMod.nextOrder = sndMod.curOrder; // Don't advance the order
        sndMod.curRow = MOD_PATTERN_ROWS;   // This triggers the break
      }
    }
    break;
  case 0x7: // Tremolo waveform
    vars->modChn->tremolo.waveform = param & 3;
    vars->modChn->tremolo.noRetrig = (param & 4) ? TRUE : FALSE;
    break;
  case 0x8: // Set panning (unsupported)
    break;
  case 0x9: // Retrigger note
    vars->modChn->retrigTick = param;
    break;
  case 0xA: // Fine volume slide up
    vars->modChn->vol = MODVolumeSlide(vars->modChn->vol, param);
    vars->updateFlags |= MOD_UPD_FLG_SET_VOL;
    break;
  case 0xB: // Fine volume slide down
    vars->modChn->vol = MODVolumeSlide(vars->modChn->vol, -param);
    vars->updateFlags |= MOD_UPD_FLG_SET_VOL;
    break;
  case 0xC: // Note cut
    vars->modChn->noteCutTick = param;
    break;
  case 0xD: // Note delay
    vars->modChn->noteDelayTick = param;
    // Don't set the mixer channel vol until the note plays
    vars->updateFlags &= ~(MOD_UPD_FLG_PLAY_NOTE | MOD_UPD_FLG_SET_VOL);
    break;
  case 0xE: // Pattern delay
    sndMod.patDelay = param;
    break;
  case 0xF: // Undefined (think of your own use for it!)
    break;
  }

} // MODFXSpecialRow

static void MODFXSpecialMid(MOD_UPDATE_VARS *vars) {
  switch (vars->modChn->param >> 4) {
  case 0x0: // Undefined, we use it for callback
    if (sndMod.callback != NULL)
      sndMod.callback(vars->modChn->param & 0xf, FALSE);
    break;
  case 0x1: // Fine porta up
    break;
  case 0x2: // Fine porta down
    break;
  case 0x3: // Glissando
    break;
  case 0x4: // Vibrato waveform
    break;
  case 0x5: // Set finetune
    break;
  case 0x6: // Pattern loop
    break;
  case 0x7: // Tremolo waveform
    break;
  case 0x8: // Set panning (unsupported)
    break;
  case 0x9: // Retrigger note
    if (--vars->modChn->retrigTick == 0) {
      vars->note = vars->modChn->note;
      vars->updateFlags |= MOD_UPD_FLG_PLAY_NOTE;
    }
    break;
  case 0xA: // Fine volume slide up
    break;
  case 0xB: // Fine volume slide down
    break;
  case 0xC: // Note cut
    if (--vars->modChn->noteCutTick == 0) {
      vars->modChn->vol = 0;
      vars->updateFlags |= MOD_UPD_FLG_SET_VOL;
      vars->modChn->effect = vars->modChn->param = 0;
    }
    break;
  case 0xD: // Note delay
    if (--vars->modChn->noteDelayTick == 0) {
      vars->note = vars->modChn->note;
      vars->updateFlags |= MOD_UPD_FLG_PLAY_NOTE | MOD_UPD_FLG_SET_VOL;
      vars->modChn->effect = vars->modChn->param = 0;
    }
    break;
  case 0xE: // Pattern delay
    break;
  case 0xF: // Undefined (think of your own use for it!)
    break;
  }

} // MODFXSpecialMid
*/

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
    data->mixCh->inc = modTiming.mixFreqPeriod / data->modCh->period;
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
