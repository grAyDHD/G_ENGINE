#ifndef TYPES_H
#define TYPES_H

//---Standard Typedefs---
#define u8 unsigned char
#define s8 signed char
typedef unsigned short u16;
#define s16 signed short
typedef unsigned int u32;
#define s32 signed long

//---Pointer Types---
typedef void (*fnptr)(void);
#define p16 u16 *
#define p32 u32 *

#define OFF 0
#define ON 1

//---Vector Types---
typedef struct {
  int x;
  int y;
} Coordinate;

#define IN_EWRAM __attribute__((section(".ewram")))
#define IN_IWRAM __attribute__((section(".iwram")))
#define ALIGN(n) __attribute__((aligned((n))))
extern u32 __FarFunction(u32 (*ptr)(), ...);
extern void __FarProcedure(u32);

#endif // TYPES_H
