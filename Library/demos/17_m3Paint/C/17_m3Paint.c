#include "../include/f.h"
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

      if (keyTapped(LT)) {
        appState = changeState(COLORS, &brush);
        break;
      } else if (keyTapped(ST)) {
        restoreFromGUICache();
        appState = DRAWING;
      }

      changeBrushSize(&brush);
      changeBrushShape(&brush);

      break;
    }
  }

  return 0;
}
