#ifndef INPUT_H
#define INPUT_H

#include "typedefs.h"

#define KEYS (*(volatile u16 *)0x4000130)
#define INPUT(k) (!(keyCache & (k)))
#define UPDATE_KEYS() (keyCache = KEYS)

// KEYS register inits to:
//  1111 1111 1111 1111
// this is the keyCache
// k is bit of input.
// keycache & k is true/1
// gba registers pressed key with 0 bit set,
// !(1) = 0, or pressed.
// that is why input macro uses ! operator
//
#define A (1 << 0)
#define B (1 << 1)
#define SL (1 << 2)
#define ST (1 << 3)
#define R (1 << 4)
#define L (1 << 5)
#define U (1 << 6)
#define D (1 << 7)
#define RS (1 << 8)
#define LS (1 << 9)

extern u16 keyCache;
extern u16 prevKeyCache;

#endif
