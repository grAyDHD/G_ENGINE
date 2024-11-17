#include "../include/f.h"
#include "draw.h"
#include "gfx.h"
#include "typedefs.h"

// Static global cache arrays
static u16 pixelCache[64][64];
static u16 guiCache[64][64];

void saveToBrushCache(Brush brush) {
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 64; y++) {
      pixelCache[x][y] = ((u16 *)VRAM)[(brush.coordinates.y + y) * SW +
                                       (brush.coordinates.x + x)];
    }
  }
}

void saveToGUICache() {
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 64; y++) {
      guiCache[x][y] = ((u16 *)VRAM)[(y * 240) + x];
    }
  }
}

void restoreFromBrushCache(Brush brush) {
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 64; y++) {
      plotPixel((brush.coordinates.x + x), (brush.coordinates.y + y),
                pixelCache[x][y]);
    }
  }
}

void restoreFromGUICache() {
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 64; y++) {
      plotPixel(x, y, guiCache[x][y]);
    }
  }
}

void draw(Brush brush) {
  switch (brush.shape) {
  case SQUARE:
    for (int x = 0; x < brush.size / 2; x++) {
      Coordinate origin = {brush.coordinates.x + x, brush.coordinates.y + x};
      drawRect(origin, (brush.size - (2 * x)), (brush.size - (2 * x)),
               brush.color);
    }
    break;
  case CIRCLE:
    drawCircle(brush.coordinates.x + brush.size,
               brush.coordinates.y + brush.size, brush.size, brush.color);
    break;
    // Other shapes can be implemented as needed.
  }
}

void erase(Brush brush) {
  switch (brush.shape) {
  case SQUARE:
    for (int x = 0; x < brush.size / 2; x++) {
      Coordinate origin = {brush.coordinates.x + x, brush.coordinates.y + x};
      drawRect(origin, (brush.size - (2 * x)), (brush.size - (2 * x)),
               brush.eraserColor);
    }
    break;
  case CIRCLE:
    drawCircle(brush.coordinates.x + brush.size,
               brush.coordinates.y + brush.size, brush.size, brush.eraserColor);
    break;
    // Other shapes can be implemented as needed.
  }
}

void handleBrushInput(Brush *brush, BrushAction action) {
  restoreFromBrushCache(*brush);

  if (keyHeld(U))
    brush->coordinates.y -= 1;
  if (keyHeld(D))
    brush->coordinates.y += 1;
  if (keyHeld(L))
    brush->coordinates.x -= 1;
  if (keyHeld(R))
    brush->coordinates.x += 1;

  switch (action) {
  case DRAW:
    draw(*brush);
    saveToBrushCache(*brush);
    break;
  case ERASE:
    erase(*brush);
    saveToBrushCache(*brush);
    draw(*brush);
    break;
  case MOVE:
    saveToBrushCache(*brush);
    draw(*brush);
    draw(*brush);
    break;
  }
  for (volatile int x = 0; x < 10000; x++)
    ;
}

Brush initiateBrush() {
  Brush brush;
  brush.size = 4;
  brush.color = RGB(0, 0, 0);
  brush.eraserColor = RGB(31, 31, 31);
  brush.shape = SQUARE;
  brush.coordinates.x = 0;
  brush.coordinates.y = 0;

  return brush;
}

enum MODE handlePause() {
  Coordinate origin = {0, 0};
  enum MODE appState = COLORS;
  saveToGUICache();
  origin.x = 0;
  origin.y = 0;
  for (int x = 32; x > 0; x--) {
    drawRect(origin, 2 * x, 2 * x, RGB(10, 10, 5));
    origin.x++;
    origin.y++;
  }
  return appState;
}

void clearScreen(Brush brush) {
  fillScreen(dblClr(brush.eraserColor));
  saveToBrushCache(brush);
}
