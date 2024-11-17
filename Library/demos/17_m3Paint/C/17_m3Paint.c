#include "../include/f.h"
#include "engine.h"
#include "in.h"
#include "typedefs.h"

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

int main() {
  DSPC = MODE3 | BG2;

  int red = 0, green = 0, blue = 0;
  Brush brush = initiateBrush();
  enum MODE appState = DRAWING;
  enum COLOR colorSelect = RED;
  Coordinate origin = {0, 0};

  clearScreen(brush);

  while (1) {
    updateKeys();
    VBLANK();

    switch (appState) {

    case (DRAWING):
      if (keyTapped(ST)) {
        appState = handlePause();
        break;
      } else if (keyDown(LT) && keyDown(RT)) {
        // if (keyDown(LT) && keyDown(RT) && keyDown(A) && keyDown(B)) {
        clearScreen(brush);
        draw(brush);
      } else if (keyTapped(A)) {
        draw(brush);
        saveToBrushCache(brush);
      } else if (keyTapped(B)) {
        erase(brush);
        saveToBrushCache(brush);
        draw(brush);
      }

      if (keyUp(A) && keyUp(B)) {
        handleBrushInput(&brush, MOVE);
      } else if (keyHeld(A)) {
        handleBrushInput(&brush, DRAW);
      } else if (keyHeld(B)) {
        handleBrushInput(&brush, ERASE);
      }

      break;

    case (COLORS):
      if (keyTapped(RT)) {
        appState = changeState(SHAPES, &brush);
        break;
      }

      drawColorSliders(colorSelect, red, green, blue);
      colorSelect = handleColorSelection(colorSelect);

      if (keyDown(D)) {
        adjustColorValue(colorSelect, &red, &green, &blue, 1);
      } else if (keyDown(U)) {
        adjustColorValue(colorSelect, &red, &green, &blue, 0);
      }

      if (keyTapped(A)) {
        brush.color = RGB(red, green, blue);
      }
      if (keyTapped(B)) {
        brush.eraserColor = RGB(red, green, blue);
      }

      drawColorDemos(brush, RGB(red, green, blue));

      for (volatile int x = 0; x < 20000; x++)
        ;
      break;
    case (SHAPES):

      origin.x = 0;
      origin.y = 0;

      if (keyTapped(LT)) {
        // changeState(COLORS);
        drawRect(origin, 64, 64, RGB(10, 0, 12));
        appState = COLORS;
        for (int x = 32; x > 0; x--) {
          drawRect(origin, 2 * x, 2 * x, RGB(10, 10, 5));
          origin.x++;
          origin.y++;
        }

      } else if (keyTapped(ST)) {
        restoreFromGUICache();
        appState = DRAWING;
      }

      origin.x = 32;
      origin.y = 32;
      if (keyTapped(U)) {
        if (brush.shape == SQUARE && brush.size < 32) {
          brush.size++;
          drawRect(origin, brush.size, brush.size, brush.color);
        } else if (brush.shape == CIRCLE && brush.size < 16) {
          brush.size++;
          drawCircle(origin.x - brush.size, origin.x - brush.size, brush.size,
                     brush.color);
        }
      } else if (keyTapped(D)) {
        if (brush.shape == SQUARE && brush.size > 2) {

          drawRect(origin, brush.size, brush.size, brush.eraserColor);
          brush.size--;
        } else if (brush.shape == CIRCLE && brush.size > 2) {
          drawCircle(origin.x - brush.size, origin.y - brush.size, brush.size,
                     brush.eraserColor);
          brush.size--;
        }
      } else if (keyTapped(R)) {
        brush.shape = CIRCLE;
        if (brush.size > 16) {
          brush.size = 16;
        }
      } else if (keyTapped(L)) {
        brush.shape = SQUARE;
      }

      break;
    }
  }

  return 0;
}
