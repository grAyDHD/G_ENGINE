#ifndef MODEFFECTS_H
#define MODEFFECTS_H

#include "modPlayer.h"

typedef enum {
  MOD_PLAY_NOTE = (1 << 0),
  MOD_SET_VOL = (1 << 1),
  MOD_SET_FREQ = (1 << 2), // create defines for bits, 1 << 1, etc
} ModUpdateFlags;

typedef enum {
  MOD_EFFECT_TIMING_ROW,
  MOD_EFFECT_TIMING_MID,

  MOD_EFFECT_TIMING_COUNT,
} ModEffectTiming;

typedef void (*ModEffect)(ModEffectUpdateData *data);

extern const ModEffect modEffect[MOD_EFFECT_TIMING_COUNT][16];
extern const ModEffectUpdateData modDefaultData[MOD_EFFECT_TIMING_COUNT];

extern void modHandleUpdateFlags(ModEffectUpdateData *data);
extern void modUpdateEffects();

//--- Mod Effect Declarations ---//

void modFxArpeggio(ModEffectUpdateData *data);
void modFxPortamentoUp(ModEffectUpdateData *data);
void modFxPortamentoDown(ModEffectUpdateData *data);
void modFxTonePortamento(ModEffectUpdateData *data);

void modFxVibrato(ModEffectUpdateData *data);
void modFxVolumeSlideTonePortamento(ModEffectUpdateData *data);
void modFxVolumeSlideVibrato(ModEffectUpdateData *data);
void modFxTremolo(ModEffectUpdateData *data);

void modFxSetPanning(ModEffectUpdateData *data);
void modFxSampleOffset(ModEffectUpdateData *data);

void modFxVolumeSlideRow(ModEffectUpdateData *data);
void modFxVolumeSlideMid(ModEffectUpdateData *data);

void modFxJumpToOrder(ModEffectUpdateData *data);

void modFxSetVolume(ModEffectUpdateData *data);
void modFxBreakToRow(ModEffectUpdateData *data);
void modFxSpecial(ModEffectUpdateData *data);
void modFxSetSpeed(ModEffectUpdateData *data);

#endif
