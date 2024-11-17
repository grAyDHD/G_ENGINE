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

void handleBrushInput(Brush *brush) {
  restoreFromBrushCache(*brush);

  if (keyDown(LT) && keyDown(RT)) {
    // if (keyDown(LT) && keyDown(RT) && keyDown(A) && keyDown(B)) {
    clearScreen(*brush);
    draw(*brush);
  } else if (keyTapped(A)) {
    draw(*brush);
    saveToBrushCache(*brush);
  } else if (keyTapped(B)) {
    erase(*brush);
    saveToBrushCache(*brush);
    draw(*brush);
  }

  if (keyHeld(U))
    brush->coordinates.y -= 1;
  if (keyHeld(D))
    brush->coordinates.y += 1;
  if (keyHeld(L))
    brush->coordinates.x -= 1;
  if (keyHeld(R))
    brush->coordinates.x += 1;

  if (keyUp(A) && keyUp(B)) {
    saveToBrushCache(*brush);
    draw(*brush);
    draw(*brush);
  } else if (keyHeld(A)) {
    draw(*brush);
    saveToBrushCache(*brush);
  } else if (keyHeld(B)) {
    erase(*brush);
    saveToBrushCache(*brush);
    draw(*brush);
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

void drawColorSliders(enum COLOR colorSelect, int red, int green, int blue) {
  Coordinate origin;

  // RED slider
  origin.x = 8;
  origin.y = 8;
  if (colorSelect == RED) {
    drawRect(origin, 8, 34, RGB(31, 31, 31));
    for (int x = 0; x < 8; x++) {
      plotPixel(origin.x + x, origin.y + red, RGB(20, 20, 20));
      plotPixel(origin.x + x, origin.y + red + 2, RGB(14, 14, 12));
    }
  } else {
    drawRect(origin, 8, 34, RGB(0, 0, 0));
  }

  origin.x = 9;
  origin.y = 9;
  for (int x = 0; x < 32; x++) {
    drawRect(origin, 6, 1, RGB(x, 0, 0));
    origin.y++;
  }

  // GREEN slider
  origin.x = 20;
  origin.y = 8;
  if (colorSelect == GREEN) {
    drawRect(origin, 8, 34, RGB(31, 31, 31));
    for (int x = 0; x < 8; x++) {
      plotPixel(origin.x + x, origin.y + green, RGB(20, 20, 20));
      plotPixel(origin.x + x, origin.y + green + 2, RGB(14, 14, 12));
    }
  } else {
    drawRect(origin, 8, 34, RGB(0, 0, 0));
  }

  origin.x = 21;
  origin.y = 9;
  for (int x = 0; x < 32; x++) {
    drawRect(origin, 6, 1, RGB(0, x, 0));
    origin.y++;
  }

  // BLUE slider
  origin.x = 32;
  origin.y = 8;
  if (colorSelect == BLUE) {
    drawRect(origin, 8, 34, RGB(31, 31, 31));
    for (int x = 0; x < 8; x++) {
      plotPixel(origin.x + x, origin.y + blue, RGB(20, 20, 20));
      plotPixel(origin.x + x, origin.y + blue + 2, RGB(14, 14, 12));
    }
  } else {
    drawRect(origin, 8, 34, RGB(0, 0, 0));
  }

  origin.x = 33;
  origin.y = 9;
  for (int x = 0; x < 32; x++) {
    drawRect(origin, 6, 1, RGB(0, 0, x));
    origin.y++;
  }
}

enum MODE changeState(enum MODE appState, Brush *brush) {
  Coordinate origin = {0, 0};
  switch (appState) {

  case SHAPES:
    appState = SHAPES;
    origin.x = 0;
    origin.y = 0;

    for (int x = 32; x > 0; x--) {
      drawRect(origin, 2 * x, 2 * x, RGB(10, 0, 12));
      origin.x++;
      origin.y++;
    }

    // drawShapeGUI, drawing rectangle size of brush.size
    origin.x = 32;
    origin.y = 32;

    if (brush->shape == SQUARE) {
      for (int x = brush->size; x > 0; x--) {
        drawRect(origin, x, x, brush->color);
        origin.x++;
        origin.y++;
      }
    } else if (brush->shape == CIRCLE) {
      drawCircle(origin.x - brush->size, origin.y - brush->size, brush->size,
                 brush->color);
    }

    break;
  case COLORS:
    drawRect(origin, 64, 64, RGB(10, 0, 12));
    appState = COLORS;
    for (int x = 32; x > 0; x--) {
      drawRect(origin, 2 * x, 2 * x, RGB(10, 10, 5));
      origin.x++;
      origin.y++;
    }
    break;
  }
  return appState;
}

void changeBrushShape(Brush *brush) {
  if (keyTapped(R)) {
    brush->shape = CIRCLE;
    if (brush->size > 16) {
      brush->size = 16;
    }
  } else if (keyTapped(L)) {
    brush->shape = SQUARE;
  }
}

void changeBrushSize(Brush *brush) {
  Coordinate origin = {32, 32};
  if (keyTapped(U)) {
    if (brush->shape == SQUARE && brush->size < 32) {
      brush->size++;
      drawRect(origin, brush->size, brush->size, brush->color);
    } else if (brush->shape == CIRCLE && brush->size < 16) {
      brush->size++;
      drawCircle(origin.x - brush->size, origin.x - brush->size, brush->size,
                 brush->color);
    }
  } else if (keyTapped(D)) {
    if (brush->shape == SQUARE && brush->size > 2) {

      drawRect(origin, brush->size, brush->size, brush->eraserColor);
      brush->size--;
    } else if (brush->shape == CIRCLE && brush->size > 2) {
      drawCircle(origin.x - brush->size, origin.y - brush->size, brush->size,
                 brush->eraserColor);
      brush->size--;
    }
  }
}

void drawColorDemos(Brush brush, int color) {
  Coordinate origin;

  // draw demo eraser
  origin.x = 10;
  origin.y = 48;
  drawRect(origin, 5, 5, brush.eraserColor);

  // draw demo brush
  origin.x = 18;
  drawRect(origin, 5, 5, brush.color);

  // draw demo RGB value
  origin.x = 40;
  drawRect(origin, 5, 5, color);
}

void adjustColorValue(enum COLOR colorSelect, int *red, int *green, int *blue,
                      int increase) {
  int *targetColor;

  // Select the target color based on colorSelect
  switch (colorSelect) {
  case RED:
    targetColor = red;
    break;
  case GREEN:
    targetColor = green;
    break;
  case BLUE:
    targetColor = blue;
    break;
  }

  if (targetColor) {
    if (increase) {
      if (*targetColor < 31) {
        (*targetColor)++;
      }
    } else {
      if (*targetColor > 0) {
        (*targetColor)--;
      }
    }
  }
}

enum COLOR handleColorSelection(enum COLOR colorSelect) {

  if (keyTapped(L)) {
    if (colorSelect == GREEN) {
      colorSelect = RED;
    } else if (colorSelect == BLUE) {
      colorSelect = GREEN;
    }
  } else if (keyTapped(R)) {
    if (colorSelect == GREEN) {
      colorSelect = BLUE;
    } else if (colorSelect == RED) {
      colorSelect = GREEN;
    }
  }

  return colorSelect;
}
