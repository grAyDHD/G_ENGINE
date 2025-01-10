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

//---Vector Types---
typedef struct {
  int x;
  int y;
} Coordinate;

#endif // TYPES_H
