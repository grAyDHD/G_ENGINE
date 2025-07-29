#ifndef MODEFFECTS_H
#define MODEFFECTS_H

#include "modPlayer.h"

// forward declaration from modPlayer.h
// struct ModEffectUpdateData;

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

extern void MODFXSpeed(ModEffectUpdateData *data);

#endif
