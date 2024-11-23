#include "../include/f.h"
#include "draw.h"
#include "gfx.h"
#include "in.h"
#include "typedefs.h"

// Static global cache arrays
static u16 pixelCache[32][32];
static u16 guiCache[64][64];

void saveToBrushCache(Brush brush) {
  for (int x = 0; x < 32; x++) {
    for (int y = 0; y < 32; y++) {
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
  for (int x = 0; x < 32; x++) {
    for (int y = 0; y < 32; y++) {
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

void paint(Brush brush) {
  switch (brush.shape) {
  case SQUARE:
    // fillSquare();
    for (int x = 0; x < brush.size / 2; x++) {
      Coordinate origin = {brush.coordinates.x + x, brush.coordinates.y + x};
      drawRect(origin, (brush.size - (2 * x)), (brush.size - (2 * x)),
               brush.color);
    }
    break;
  case CIRCLE:
    // fillCircle();
    fillCircle(brush.coordinates.x + brush.size,
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
    fillCircle(brush.coordinates.x + brush.size,
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
    paint(*brush);
  }

  if (keyHeld(U))
    brush->coordinates.y -= 1;
  if (keyHeld(D))
    brush->coordinates.y += 1;
  if (keyHeld(L))
    brush->coordinates.x -= 1;
  if (keyHeld(R))
    brush->coordinates.x += 1;

  // Paint or erase only if LT and RT are NOT held
  if (!keyDown(LT) && !keyDown(RT)) {
    if (keyTapped(A)) {
      paint(*brush);
      saveToBrushCache(*brush);
    } else if (keyTapped(B)) {
      erase(*brush);
      saveToBrushCache(*brush);
      paint(*brush);
    }
  }

  if (keyUp(A) && keyUp(B)) { // move brush and show position
    saveToBrushCache(*brush);
    paint(*brush);

  } else if (keyHeld(A)) { // draw brush at current position
    if (keyDown(RT)) {     // draw with symmetry
      symmetryPaint(*brush);
      saveToBrushCache(*brush);
    } else if (keyDown(LT)) {
      // draw gradient shape
      paintGradient(*brush);
      saveToBrushCache(*brush);
    } else { // draw
      paint(*brush);
      saveToBrushCache(*brush);
    }

  } else if (keyHeld(B)) {
    if (keyDown(RT)) { // draw with symmetry
                       // symmetryErase(*brush)
    } else {           // erase brush at current position
      erase(*brush);
      saveToBrushCache(*brush);
      paint(*brush);
    }
  }

  for (volatile int x = 0; x < 10000; x++)
    ;
}

void paintGradient(Brush brush) {
  Coordinate origin = brush.coordinates;
  Coordinate endPoint = brush.coordinates;
  // only handles SQUARE brush for time being
  // get RGB values from brush.color to construct gradients
  int red = GET_RED(brush.color);
  int green = GET_RED(brush.color);
  int blue = GET_RED(brush.color);

  switch (brush.shape) {
  case SQUARE:
    switch (brush.gradient) {
    case VERTICAL:
      endPoint.y += brush.size;
      for (int x = 0; x < brush.size; x++) {
        red += (x * brush.gradientScaleR);
        green += (x * brush.gradientScaleG);
        blue += (x * brush.gradientScaleB);

        drawLine(origin, endPoint, RGB(red, green, blue));
        origin.x++;
        endPoint.x++;
      }
      break;

    case PERIMETER:
      for (int x = 0; x < brush.size / 2; x++) {
        origin.x += 1;
        origin.y += 1;

        red += (x * brush.gradientScaleR);
        green += (x * brush.gradientScaleG);
        blue += (x * brush.gradientScaleB);
        drawRect(origin, (brush.size - (2 * x)), (brush.size - (2 * x)),
                 RGB(red, green, blue));
      }
      break;

    case HORIZONTAL:
      endPoint.x += brush.size;

      for (int x = 0; x < brush.size; x++) {
        red += (x * brush.gradientScaleR);
        green += (x * brush.gradientScaleG);
        blue += (x * brush.gradientScaleB);

        drawLine(origin, endPoint, RGB(red, green, blue));
        origin.y++;
        endPoint.y++;
      }
      break;
    }
    break;

  case CIRCLE:
    switch (brush.gradient) {
    case VERTICAL: {
      int radius = brush.size;
      int r = radius, s = 0;
      int decisionOver2 = 1 - r;

      while (r >= s) {
        unsigned short currentColor = RGB(red, green, blue);

        // Draw vertical lines across the circle
        // Right half
        drawVerticalLine(origin.y - r + brush.size, origin.y + r + brush.size,
                         origin.x + s + brush.size,
                         currentColor); // Vertical line at x + s
        drawVerticalLine(origin.y - s + brush.size, origin.y + s + brush.size,
                         origin.x + r + brush.size,
                         currentColor); // Vertical line at x + r

        // Left half
        drawVerticalLine(origin.y - r + brush.size, origin.y + r + brush.size,
                         origin.x - s + brush.size,
                         currentColor); // Vertical line at x - s
        drawVerticalLine(origin.y - s + brush.size, origin.y + s + brush.size,
                         origin.x - r + brush.size,
                         currentColor); // Vertical line at x - r

        // Update gradient color for the next lines (vertical change)
        red += brush.gradientScaleR;
        green += brush.gradientScaleG;
        blue += brush.gradientScaleB;

        // Adjust the decision variable for the circle
        s++;
        if (decisionOver2 <= 0) {
          decisionOver2 += 2 * s + 1;
        } else {
          r--;
          decisionOver2 += 2 * (s - r) + 1;
        }
      }
      break;
    }
    case PERIMETER:
      for (int x = brush.size; x > 0; x--) {
        drawCircle(origin.x + brush.size, origin.y + brush.size, x,
                   RGB(red, green, blue));
        red += brush.gradientScaleR;
        green += brush.gradientScaleG;
        blue += brush.gradientScaleB;
      }
      break;

    case HORIZONTAL: {
      int radius = brush.size;
      int r = radius, s = 0;
      int decisionOver2 = 1 - r;

      while (r >= s) {
        // Adjust the gradient for each horizontal step
        unsigned short currentColor = RGB(red, green, blue);
        //    drawHorizontalLine(int x_start, int x_end, int y, unsigned short
        //    color)

        // Top half lines
        drawHorizontalLine(origin.x - r + brush.size, origin.x + r + brush.size,
                           origin.y + s + brush.size, currentColor);
        drawHorizontalLine(origin.x - s + brush.size, origin.x + s + brush.size,
                           origin.y + r + brush.size, currentColor);

        // Bottom half lines
        drawHorizontalLine(origin.x - r + brush.size, origin.x + r + brush.size,
                           origin.y - s + brush.size, currentColor);
        drawHorizontalLine(origin.x - s + brush.size, origin.x + s + brush.size,
                           origin.y - r + brush.size, currentColor);

        // Update gradient color for the next lines (horizontal change)
        red += brush.gradientScaleR;
        green += brush.gradientScaleG;
        blue += brush.gradientScaleB;

        // Adjust the decision variable for the circle
        s++;
        if (decisionOver2 <= 0) {
          decisionOver2 += 2 * s + 1;
        } else {
          r--;
          decisionOver2 += 2 * (s - r) + 1;
        }
      }
      break;
    }
    }
    break;
  }
}

void symmetryPaint(Brush brush) {
  Coordinate flipX = {(SW - brush.coordinates.x - brush.size),
                      brush.coordinates.y};
  Coordinate flipY = {brush.coordinates.x,
                      (SH - brush.coordinates.y - brush.size)};
  Coordinate flipXY = {(SW - brush.coordinates.x - brush.size),
                       (SH - brush.coordinates.y - brush.size)};

  switch (brush.shape) {
  case SQUARE:
    drawRect(brush.coordinates, brush.size, brush.size, brush.color);
    drawRect(flipX, brush.size, brush.size, brush.color);
    drawRect(flipY, brush.size, brush.size, brush.color);
    drawRect(flipXY, brush.size, brush.size, brush.color);

    /*
    // BRUSH IS IN Q1
  if (brush.coordinates.x >= 0 && brush.coordinates.y >= 0 &&
      brush.coordinates.x <= ((SW / 2) - brush.size) &&
      brush.coordinates.y <= ((SH / 2) - brush.size)) {
  } else if (brush.coordinates.x >= (SW / 2) && brush.coordinates.y >= 0 &&
             brush.coordinates.x <= (SW - brush.size) &&
             brush.coordinates.y <= ((SH / 2) - brush.size)) {
  } else if (brush.coordinates.x >= 0 && brush.coordinates.y >= (SH / 2) &&
             brush.coordinates.x <= ((SW / 2) - brush.size) &&
             brush.coordinates.y <= (SH - brush.size)) {
  } else if (brush.coordinates.x >= (SW / 2) &&
             brush.coordinates.y >= (SH / 2) &&
             brush.coordinates.x <= (SW - brush.size) &&
             brush.coordinates.y <= (SH - brush.size)) {
  }

  */

    /*
  for (int x = 0; x < brush.size / 2; x++) {
    Coordinate origin = {brush.coordinates.x + x, brush.coordinates.y + x};
    drawRect(origin, (brush.size - (2 * x)), (brush.size - (2 * x)),
             brush.color);
  }
        red += (x * brush.gradientScaleR);
        green += (x * brush.gradientScaleG);
        blue += (x * brush.gradientScaleB);
        red += (x * brush.gradientScaleR);
        green += (x * brush.gradientScaleG);
        blue += (x * brush.gradientScaleB);
        red += (x * brush.gradientScaleR);
        green += (x * brush.gradientScaleG);
        blue += (x * brush.gradientScaleB);
    */
    break;
  case CIRCLE:
    fillCircle(brush.coordinates.x + brush.size,
               brush.coordinates.y + brush.size, brush.size, brush.color);
    fillCircle(flipX.x + brush.size, flipX.y + brush.size, brush.size,
               brush.color);
    fillCircle(flipY.x + brush.size, flipY.y + brush.size, brush.size,
               brush.color);
    fillCircle(flipXY.x + brush.size, flipXY.y + brush.size, brush.size,
               brush.color);
    break;
    // Other shapes can be implemented as needed.
  }
}

void manageGradientType(Brush *brush) {
  if (keyTapped(L)) {
    if (brush->gradient == PERIMETER) {
      brush->gradient = VERTICAL;
    } else if (brush->gradient == HORIZONTAL) {
      brush->gradient = PERIMETER;
    }

  } else if (keyTapped(R)) {
    if (brush->gradient == PERIMETER) {
      brush->gradient = HORIZONTAL;
    } else if (brush->gradient == VERTICAL) {
      brush->gradient = PERIMETER;
    }
  }
}

Brush initiateBrush() {
  Brush brush;
  brush.size = 4;
  brush.color = RGB(0, 0, 0);
  brush.eraserColor = RGB(31, 31, 31);
  brush.shape = SQUARE;
  brush.coordinates.x = 0;
  brush.coordinates.y = 0;
  brush.gradient = PERIMETER;

  brush.gradientScaleR = 4;
  brush.gradientScaleG = 2;
  brush.gradientScaleB = 1;
  return brush;
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
      fillCircle(origin.x - brush->size, origin.y - brush->size, brush->size,
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
  case GRADIENTS:

    for (int x = 32; x > 0; x--) {
      drawRect(origin, 2 * x, 2 * x, RGB(20, 20, 30));
      origin.x++;
      origin.y++;
    }
    appState = GRADIENTS;
    break;
  }
  return appState;
}

void changeBrushShape(Brush *brush) {
  if (keyTapped(R)) {
    brush->shape = CIRCLE;
    if (brush->size > 8) {
      brush->size = 8;
    }
  } else if (keyTapped(L)) {
    brush->shape = SQUARE;
  }
}

void changeBrushSize(Brush *brush) {
  Coordinate origin = {32, 32};
  if (keyTapped(U)) {
    if (brush->shape == SQUARE && brush->size < 16) {
      brush->size++;
      drawRect(origin, brush->size, brush->size, brush->color);
    } else if (brush->shape == CIRCLE && brush->size < 8) {
      brush->size++;
      fillCircle(origin.x - brush->size, origin.x - brush->size, brush->size,
                 brush->color);
    }
  } else if (keyTapped(D)) {
    if (brush->shape == SQUARE && brush->size > 2) {

      drawRect(origin, brush->size, brush->size, brush->eraserColor);
      brush->size--;
    } else if (brush->shape == CIRCLE && brush->size > 2) {
      fillCircle(origin.x - brush->size, origin.y - brush->size, brush->size,
                 brush->eraserColor);
      brush->size--;
    }
  }
}

void drawColorPreview(Brush brush, int color) {
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

void handleGradientControls(enum COLOR colorSelect, Brush *brush) {
  if (keyTapped(A)) { // this would be a nice spot for a tribool!!!!
    switch (brush->gradient) {
    case HORIZONTAL:
      brush->gradient = VERTICAL;
      break;
    case VERTICAL:
      brush->gradient = PERIMETER;
      break;
    case PERIMETER:
      brush->gradient = HORIZONTAL;
      break;
    }
  } else if (keyTapped(B)) {
    switch (brush->gradient) {
    case HORIZONTAL:
      brush->gradient = PERIMETER;
      break;
    case VERTICAL:
      brush->gradient = HORIZONTAL;
      break;
    case PERIMETER:
      brush->gradient = VERTICAL;
      break;
    }
  } else if (keyTapped(U)) {
    switch (colorSelect) {
    case RED:
      if (brush->gradientScaleR < 4) {
        brush->gradientScaleR++;
      }
      break;
    case GREEN:
      if (brush->gradientScaleG < 4) {
        brush->gradientScaleG++;
      }
      break;
    case BLUE:
      if (brush->gradientScaleB < 4) {
        brush->gradientScaleB++;
      }
      break;
    }
  } else if (keyTapped(D)) {
    switch (colorSelect) {
    case RED:
      if (brush->gradientScaleR > 0) {
        brush->gradientScaleR--;
      }
      break;
    case GREEN:
      if (brush->gradientScaleG > 0) {
        brush->gradientScaleG--;
      }
      break;
    case BLUE:
      if (brush->gradientScaleB > 0) {
        brush->gradientScaleB--;
      }
      break;
    }
  }
}

void drawGradientsGUI(enum COLOR colorSelect, Brush brush) {
  Coordinate origin = {12, 56};

  drawRect(origin, 4, 4, RGB(31, 0, 0));
  if (colorSelect == RED) {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 31, 31));
    origin.x--;
    origin.y--;
  } else {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 0, 0));
    origin.x--;
    origin.y--;
  }
  origin.x += 12;

  drawRect(origin, 4, 4, RGB(0, 31, 0));
  if (colorSelect == GREEN) {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 31, 31));
    origin.x--;
    origin.y--;
  } else {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 0, 0));
    origin.x--;
    origin.y--;
  }
  origin.x += 12;

  drawRect(origin, 4, 4, RGB(0, 0, 31));
  if (colorSelect == BLUE) {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 31, 31));
    origin.x--;
    origin.y--;
  } else {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 0, 0));
    origin.x--;
    origin.y--;
  }

  // draw gradient bar levels
  origin.x = 12; // y = 56
  for (int x = 0; x < 4; x++) {
    origin.y -= 6;
    if (brush.gradientScaleR >= x) {
      drawRect(origin, 4, 2, 0);
    } else {
      drawRect(origin, 4, 2, RGB(20, 20, 30));
    }
  }

  origin.x += 12;
  origin.y = 56; // y = 56
  for (int x = 0; x < 4; x++) {
    origin.y -= 6;
    if (brush.gradientScaleG >= x) {
      drawRect(origin, 4, 2, 0);
    } else {
      drawRect(origin, 4, 2, RGB(20, 20, 30));
    }
  }
  origin.x += 12;

  origin.x += 12;
  origin.y = 56; // y = 56
  for (int x = 0; x < 4; x++) {
    origin.y -= 6;
    if (brush.gradientScaleB >= x) {
      drawRect(origin, 4, 2, 0);
    } else {
      drawRect(origin, 4, 2, RGB(20, 20, 30));
    }
  }
}

void fillCircle(int x, int y, int radius, unsigned short color) {
  int r = radius;
  int s = 0;
  int decisionOver2 =
      1 - r; // Decision variable for the midpoint circle algorithm

  // Iterate until the step crosses the radius
  while (r >= s) {

    // Draw horizontal lines between mirrored points at each step
    // Top half
    drawHorizontalLine(x - r, x + r, y + s, color); // Line at y + s
    drawHorizontalLine(x - s, x + s, y + r, color); // Line at y + r

    // Bottom half
    drawHorizontalLine(x - r, x + r, y - s, color); // Line at y - s
    drawHorizontalLine(x - s, x + s, y - r, color); // Line at y - r
    //

    // Update s and adjust the decision variable
    s++;
    if (decisionOver2 <= 0) {
      // Midpoint is within the circle
      decisionOver2 += 2 * s + 1;
    } else {
      // Midpoint is outside the circle, reduce r
      r--;
      decisionOver2 += 2 * (s - r) + 1;
    }
  }
}

// Helper function to draw a horizontal line from x_start to x_end at a specific
// y
void drawHorizontalLine(int x_start, int x_end, int y, unsigned short color) {
  for (int x = x_start; x <= x_end; x++) {
    plotPixel(x, y, color); // Plot each pixel on the horizontal line
  }
}

void drawVerticalLine(int y_start, int y_end, int x, unsigned short color) {
  for (int y = y_start; y <= y_end; y++) {
    plotPixel(x, y, color); // Plot each pixel on the vertical line
  }
}

void simpleWait(int delay) {
  for (volatile int z = 0; z < (delay * 1000); z++)
    ;
}
