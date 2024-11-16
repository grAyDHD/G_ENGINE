#ifndef F_H
#define F_H

#include "draw.h"     // Provides drawRect, drawCircle, and plotPixel.
#include "engine.h"   // Includes VRAM, SW definitions.
#include "gfx.h"      // For graphical operations.
#include "in.h"       // For input handling (keyHeld, etc.).
#include "typedefs.h" // Ensure this includes Coordinate and necessary typedefs.

#define VRAM_SCREEN_END 0x06012C00
#define CURSOR_CACHE ((u16)VRAM_SCREEN_END)

// Enumerations for modes, colors, and shapes
enum MODE { DRAWING = 0, COLORS, SHAPES };
enum COLOR { RED = 0, GREEN, BLUE };
enum SHAPE { SQUARE, TRIANGLE, CIRCLE, HEXAGON };

// Brush struct definition
typedef struct {
  Coordinate coordinates;
  enum SHAPE shape;
  u16 size; // Max size of 32
  u16 color;
  u16 eraserColor;
} Brush;

// Function prototypes
void saveToBrushCache(Brush brush);
void saveToGUICache();
void restoreFromBrushCache(Brush brush);
void restoreFromGUICache();
void draw(Brush brush);
void erase(Brush brush);
void updateBrushPosition(Coordinate *cursor);

#endif // F_H
