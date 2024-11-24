#include "../include/brush.h"
#include "../include/drw.h"
#include "../include/state.h"
#include "../include/types.h"
#include "draw.h"
#include "gfx.h"
#include "in.h"

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
  // restores area BEHIND brush, on canvas
  // rename to restore from canvas cache?
  // so, flickering happens by this writing to screen then drawing brush right
  // after
  restoreFromBrushCache(*brush);

  if (keyDown(LT) && keyDown(RT)) {
    // if (keyDown(LT) && keyDown(RT) && keyDown(A) && keyDown(B)) {
    clearScreen(*brush);
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
  }
}
