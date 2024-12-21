#include "input/in.h"
#include "core/typedefs.h"
#include "graphics/video.h"

#define ALIGN4 __attribute__((aligned(4)))
#define KEY_MASK 0x03FF

typedef struct REPEAT_REC {
  u16 keys;  //!< Repeated keys.
  u16 mask;  //!< Only check repeats for these keys.
  u8 count;  //!< Repeat counter.
  u8 delay;  //!< Limit for first repeat.
  u8 repeat; //!< Limit for successive repeats.
} ALIGN4 REPEAT_REC;

u16 keyCache = 0, prevKeyCache = 0;
REPEAT_REC __key_rpt = {0, KEY_MASK, 60, 60, 30};

void updateKeys(void) {
  prevKeyCache = keyCache;
  keyCache = ~KEYS & KEY_MASK;

  REPEAT_REC *rpt = &__key_rpt;
  rpt->keys = 0; // Clear repeats again

  if (rpt->delay) {
    if (keyChanged(rpt->mask)) {
      rpt->count = rpt->delay;
      rpt->keys = keyCache;
    } else
      rpt->count--;

    if (rpt->count == 0) {
      rpt->count = rpt->repeat;
      rpt->keys = keyCache & rpt->mask;
    }
  }
}

void waitForInput(u16 key) {
  while (1) {
    VBLANK();
    updateKeys();
    if (keyTapped(key))
      return;
  }
}

u32 key_repeat(u32 keys) { return __key_rpt.keys & keys; }
void key_repeat_mask(u32 mask) { __key_rpt.mask = mask & KEY_MASK; }

/*
        param delay	Set first repeat limit. If 0, repeats are off.
        param repeat	Sets later repeat limit.
        note	Both limits have a range of [0, 255]. If either argument
          is <0, the old value will be kept.
*/
void key_repeat_limits(int delay, int repeat) {
  REPEAT_REC *rpt = &__key_rpt;

  if (delay >= 0)
    rpt->delay = delay;
  if (repeat >= 0)
    rpt->repeat = repeat;
  rpt->count = delay; // Reset counter.
}
