#ifndef TYPES_H
#define TYPES_H

//---Standard Typedefs---
#define u8  unsigned char
#define s8  signed char
#define u16 unsigned short
#define s16 signed short
#define u32 unsigned long
#define s32 signed long

//---Pointer Types---
#define p8  u8*
#define p16 u16*
#define p32 u32*

//---Floating-Point Types---
#define f32 float
#define f64 double

//---Fixed-Point Macros---
#define FIXED_16_16(x) ((x) << 16)  // Convert integer to fixed-point 16.16
#define FIXED_TO_INT(x) ((x) >> 16) // Convert fixed-point 16.16 to integer

//---RGB Color---
#define RGB(r, g, b)  ((r) | ((g) << 5) | ((b) << 10))

//---Vector Types---
typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vector3;

#define VECTOR2(x, y)  ((Vector2){(x), (y)})
#define VECTOR3(x, y, z)  ((Vector3){(x), (y), (z)})

//---Boolean Type---
#define BOOL u8
#define TRUE 1
#define FALSE 0

#endif // TYPES_H
