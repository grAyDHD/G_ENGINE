#ifndef TYPE_H
#define TYPE_H

// Enumerations for modes, colors, and shapes
#include "core/typedefs.h"

#define VRAM_SCREEN_END 0x06012C00
#define CURSOR_CACHE ((u16)VRAM_SCREEN_END)

enum MODE { DRAWING, COLORS, SHAPES, GRADIENTS };
enum COLOR { RED, GREEN, BLUE };
enum SHAPE { SQUARE, CIRCLE, HEXAGON };
enum GRADIENT { HORIZONTAL, VERTICAL, PERIMETER };

typedef enum { MOVE, DRAW, ERASE } BrushAction;

// Brush struct definition
typedef struct {
  Coordinate coordinates;
  enum SHAPE shape;
  enum GRADIENT gradient;
  u16 size; // Max size of 32
  u16 color;
  u16 eraserColor;

  int gradientScaleR;
  int gradientScaleG;
  int gradientScaleB;
} Brush;
#endif
