#ifndef MATH_H
#define MATH_H

#define INT_TO_FIXED(i) (i << 16)
#define FIXED_TO_INT(f) (f >> 16)

#define MUL_FINTEGER(x, y)                                                     \
  (((x >> 16) * (y >> 16)) << 16) // clear lower bits to multiply integer
                                  // portion
#define MUL_FFRACTION(x, y) (((x & 0x0000FFFF) * (y & 0x0000FFFF)) >> 16)
#define MUL(x, y) (s32)(MUL_FINTEGER(x, y) | MUL_FFRACTION(x, y))

#define MULT(x, y) ((s32)((((long long)(x)) * ((long long)(y))) >> 16))

#define FIXED_ONE (1 << 16)
#define FIXED_HALF (1 << 15)
#define FIXED_QUARTER (2 << 14)

#define GRAVITY INT_TO_FIXED(256)

#endif // !MATH_H
