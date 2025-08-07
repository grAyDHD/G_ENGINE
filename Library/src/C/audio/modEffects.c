#include "audio/modEffects.h"
#include "audio/modPlayer.h"

void modFxArpeggio(ModEffectUpdateData *data) {}

void modFxPortamentoUp(ModEffectUpdateData *data) {}

void modFxPortamentoDown(ModEffectUpdateData *data) {}

void modFxTonePortamento(ModEffectUpdateData *data) {}

void modFxVibrato(ModEffectUpdateData *data) {}

void modFxVolumeSlideTonePortamento(ModEffectUpdateData *data) {}

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
  data->modCh->vol =
      modFxVolumeSlide(data->modCh->vol, data->modCh->volumeSlideSpeed);
  data->updateFlags |= MOD_SET_VOL;
}

void modFxJumpToOrder(ModEffectUpdateData *data) {}

void modFxSetVolume(ModEffectUpdateData *data) {
  data->modCh->vol = data->param;
  if (data->modCh->vol > 64) {
    data->modCh->vol = 64;
  }
}

void modFxBreakToRow(ModEffectUpdateData *data) {}

void modFxSpecialRow(ModEffectUpdateData *data) {
  u32 param = data->modCh->param & 0xF;

  switch (data->modCh->param >> 4) {
  case 0x0:
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

void modFxSpecialMid(ModEffectUpdateData *data) {
  u32 param = data->modCh->param & 0xF;

  switch (data->modCh->param >> 4) {
  case 0x0: // callback
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

void modFxSetSpeed(ModEffectUpdateData *data) {
  if (data->param < 32) {
    modPlayer.speed = data->param;
  }
}

const ModEffect modEffect[MOD_EFFECT_TIMING_COUNT][16] = {
    {
        // MOD_EFFECT_TABLE_ROW
        NULL,                // 0x0: Arpeggio
        NULL,                // 0x1: Porta up
        NULL,                // 0x2: Porta down
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
        NULL,                // 0x0: Arpeggio
        NULL,                // 0x1: Porta up
        NULL,                // 0x2: Porta down
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
