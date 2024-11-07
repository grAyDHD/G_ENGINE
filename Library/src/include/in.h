#ifndef TONC_INPUT
#define TONC_INPUT
#include "typedefs.h"

#define KEYS *(volatile u16 *)(0x4000000 + 0x0130)
#define INLINE inline
#define KEY_FULL 0xFFFFFFFF //!< Define for checking all keys.

#define A 0x0001
#define B 0x0002
#define SL 0x0004
#define ST 0x0008
#define R 0x0010
#define L 0x0020
#define U 0x0040
#define D 0x0080
#define RT 0x0100
#define LT 0x0200

#define KEY_DOWN(key) (~(KEYS) & key)
#define KEY_UP(key) ((KEYS) & key)

// test whether all keys are pressed, released, whatever.
// Example use:
//   KEY_EQ(key_hit, KEY_L | KEY_R)
// will be true if and only if KEY_L and KEY_R are _both_ being pressed
#define KEY_EQ(key_fun, keys) (key_fun(keys) == (keys))

#define KEY_TRIBOOL(fnKey, plus, minus)                                        \
  (bit_tribool(fnKey(KEY_FULL), plus, minus))

// --------------------------------------------------------------------
// GLOBALS
// --------------------------------------------------------------------

extern u16 keyCache, prevKeyCache;
// --------------------------------------------------------------------
// PROTOTYPES
// --------------------------------------------------------------------

void key_wait_for_clear(u32 key); // wait for keys to be up

//! \name Basic synchronous keystates
//\{
void key_poll();
INLINE u32 key_curr_state(void);
INLINE u32 key_prev_state(void);

INLINE u32 key_is_down(u32 key);
INLINE u32 key_is_up(u32 key);

INLINE u32 key_was_down(u32 key);
INLINE u32 key_was_up(u32 key);
//\}

//! \name Transitional keystates
//\{
INLINE u32 key_transit(u32 key);
INLINE u32 key_held(u32 key);
INLINE u32 key_hit(u32 key);
INLINE u32 key_released(u32 key);
//\}

//! \name Tribools
//\{
// INLINE int key_tri_horz(void);
// INLINE int key_tri_vert(void);
// INLINE int key_tri_shoulder(void);
// INLINE int key_tri_fire(void);
//\}

//! \name Key repeats
//\{
u32 key_repeat(u32 keys);

void key_repeat_mask(u32 mask);
void key_repeat_limits(int delay, int repeat);
//\}

void key_wait_till_hit(u16 key);

// --------------------------------------------------------------------
// INLINES
// --------------------------------------------------------------------

//! Get current keystate
INLINE u32 key_curr_state(void) { return keyCache; }

//! Get previous key state
INLINE u32 key_prev_state(void) { return prevKeyCache; }

//! Gives the keys of \a key that are currently down
INLINE u32 key_is_down(u32 key) { return keyCache & key; }

//! Gives the keys of \a key that are currently up
INLINE u32 key_is_up(u32 key) { return ~keyCache & key; }

//! Gives the keys of \a key that were previously down
INLINE u32 key_was_down(u32 key) { return prevKeyCache & key; }

//! Gives the keys of \a key that were previously down
INLINE u32 key_was_up(u32 key) { return ~prevKeyCache & key; }

//! Gives the keys of \a key that are different from before
INLINE u32 key_transit(u32 key) { return (keyCache ^ prevKeyCache) & key; }

//! Gives the keys of \a key that are being held down
INLINE u32 key_held(u32 key) { return (keyCache & prevKeyCache) & key; }

//! Gives the keys of \a key that are pressed (down now but not before)
INLINE u32 key_hit(u32 key) { return (keyCache & ~prevKeyCache) & key; }

//! Gives the keys of \a key that are being released
INLINE u32 key_released(u32 key) { return (~keyCache & prevKeyCache) & key; }

// Horizontal tribool (right,left)=(+,-)
INLINE int key_tri_horz(void) {
  return bit_tribool(keyCache, KI_RIGHT, KI_LEFT);
}

// Vertical tribool (down,up)=(+,-)
INLINE int key_tri_vert(void) { return bit_tribool(keyCache, KI_DOWN, KI_UP); }

// Shoulder-button tribool (R,L)=(+,-)
INLINE int key_tri_shoulder(void) { return bit_tribool(keyCache, KI_R, KI_L); }

// Fire-button tribool (A,B)=(+,-)
INLINE int key_tri_fire(void) { return bit_tribool(keyCache, KI_A, KI_B); }

#endif
