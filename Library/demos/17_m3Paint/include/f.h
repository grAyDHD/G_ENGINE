#ifndef F_H
#define F_H

#include "draw.h"     // Provides drawRect, drawCircle, and plotPixel.
#include "engine.h"   // Includes VRAM, SW definitions.
#include "gfx.h"      // For graphical operations.
#include "in.h"       // For input handling (keyHeld, etc.).
#include "typedefs.h" // Ensure this includes Coordinate and necessary typedefs.

#define VRAM_SCREEN_END 0x06012C00
#define CURSOR_CACHE ((u16)VRAM_SCREEN_END)

#define GET_RED(color) (color & 0x1F)
#define GET_GREEN(color) ((color >> 5) & 0x1F)
#define GET_BLUE(color) ((color >> 10) & 0x1F)

// Enumerations for modes, colors, and shapes
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

// Function prototypes
void saveToBrushCache(Brush brush);
void saveToGUICache();
void restoreFromBrushCache(Brush brush);
void restoreFromGUICache();
void draw(Brush brush);
void erase(Brush brush);
void updateBrushPosition(Brush *brush);
Brush initiateBrush();
enum MODE handlePause();
void clearScreen(Brush brush);
void handleBrushInput(Brush *brush);
void drawColorSliders(enum COLOR colorSelect, int red, int green, int blue);
enum MODE changeState(enum MODE appState, Brush *brush);
void changeBrushShape(Brush *brush);
void changeBrushSize(Brush *brush);
void drawColorPreview(Brush brush, int color);

void adjustColorValue(enum COLOR colorSelect, int *red, int *green, int *blue,
                      int increase);

enum COLOR handleColorSelection(enum COLOR colorSelect);
void symmetryPaint(Brush brush);
void manageGradientType(Brush *brush);
void paintGradient(Brush brush);

void handleGradientControls(enum COLOR colorSelect, Brush *brush);
void drawGradientsGUI(enum COLOR colorSelect, Brush brush);

void fillCircle(int x, int y, int radius, unsigned short color);
void drawHorizontalLine(int x_start, int x_end, int y, unsigned short color);
void drawVerticalLine(int y_start, int y_end, int x, unsigned short color);

void simpleWait(int delay);

#endif // F_H
