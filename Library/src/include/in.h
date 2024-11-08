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

#define KEY_EQ(key_fun, keys) (key_fun(keys) == (keys))

extern u16 keyCache, prevKeyCache;

void waitForNoInput(u32 key); // wait for keys to be up
void waitForInput(u16 key);

void updateKeys();

u32 key_repeat(u32 keys);
void key_repeat_mask(u32 mask);
void key_repeat_limits(int delay, int repeat);

INLINE u32 keyState(void) { return keyCache; }
INLINE u32 prevKeyState(void) { return prevKeyCache; }
INLINE u32 keyDown(u32 key) { return keyCache & key; }
INLINE u32 keyUp(u32 key) { return ~keyCache & key; }
INLINE u32 keyWasDown(u32 key) { return prevKeyCache & key; }
INLINE u32 keyWasUp(u32 key) { return ~prevKeyCache & key; }
INLINE u32 keyChanged(u32 key) { return (keyCache ^ prevKeyCache) & key; }
INLINE u32 keyHeld(u32 key) { return (keyCache & prevKeyCache) & key; }
INLINE u32 keyTapped(u32 key) { return (keyCache & ~prevKeyCache) & key; }
INLINE u32 keyReleased(u32 key) { return (~keyCache & prevKeyCache) & key; }

// INLINE int key_tri_horz(void) {
// return bit_tribool(keyCache, KI_RIGHT, KI_LEFT);
//}

// Vertical tribool (down,up)=(+,-)
// INLINE int key_tri_vert(void) { return bit_tribool(keyCache, KI_DOWN, KI_UP);
// }

// Shoulder-button tribool (R,L)=(+,-)
// INLINE int key_tri_shoulder(void) { return bit_tribool(keyCache, KI_R, KI_L);
// }

// Fire-button tribool (A,B)=(+,-)
// INLINE int key_tri_fire(void) { return bit_tribool(keyCache, KI_A, KI_B); }

#endif
