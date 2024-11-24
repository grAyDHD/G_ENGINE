#include "../include/state.h"
#include "../include/drw.h"
#include "../include/types.h"
#include "draw.h"
#include "gfx.h"
#include "in.h"

static u16 pixelCache[32][32];
static u16 guiCache[64][64];

void saveToBrushCache(Brush brush) {
  for (int x = 0; x < brush.size; x++) {
    for (int y = 0; y < brush.size; y++) {
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
  for (int x = 0; x < brush.size; x++) {
    for (int y = 0; y < brush.size; y++) {
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
  case DRAWING:
    restoreFromGUICache();
    appState = DRAWING;
    break;

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

void simpleWait(int delay) {
  for (volatile int z = 0; z < (delay * 1000); z++)
    ;
}
