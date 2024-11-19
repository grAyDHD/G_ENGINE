#include "../include/f.h"
#include "draw.h"
#include "engine.h"
#include "in.h"
#include "typedefs.h"

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
      }

      handleBrushInput(&brush);
      break;

    case (COLORS):
      if (keyDown(RT) && keyDown(LT)) {
        appState = changeState(GRADIENTS, &brush);
        break;
      } else if (keyTapped(RT)) {
        appState = changeState(SHAPES, &brush);
        break;
      } else if (keyTapped(ST)) {
        restoreFromGUICache();
        appState = DRAWING;
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

      drawColorPreview(brush, RGB(red, green, blue));

      for (volatile int x = 0; x < 20000; x++)
        ;
      break;
    case (SHAPES):

      if (keyDown(LT) && keyDown(RT)) {
        appState = changeState(GRADIENTS, &brush);
      } else if (keyTapped(LT)) {
        appState = changeState(COLORS, &brush);
        break;
      } else if (keyTapped(ST)) {
        restoreFromGUICache();
        appState = DRAWING;
      }

      changeBrushSize(&brush);
      changeBrushShape(&brush);
      break;
    case (GRADIENTS):

      colorSelect = handleColorSelection(colorSelect);
      handleGradientControls(colorSelect, &brush);

      // todo- in GUI function, align black squares, remove black squares;
      drawGradientsGUI(colorSelect, brush);

      origin = brush.coordinates;
      brush.coordinates.x = 0;
      brush.coordinates.y = 0;
      paintGradient(brush);
      brush.coordinates = origin;

      if (keyTapped(LT)) {
        appState = changeState(COLORS, &brush);
      } else if (keyTapped(RT)) {
        appState = changeState(SHAPES, &brush);
      } else if (keyTapped(ST)) {
        appState = changeState(DRAWING, &brush);
      }

      break;
    }
  }

  return 0;
}
