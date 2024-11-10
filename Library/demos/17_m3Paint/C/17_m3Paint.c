#include "engine.h"
#include "gfx.h"
#include "in.h"
#include "typedefs.h"

enum MODE { DRAWING = 0, COLOR, SHAPE } MODE;
enum COLOR_SELECT { RED = 0, GREEN, BLUE };
enum SHAPE_SELECT { SQUARE, TRIANGLE, CIRCLE, HEXAGON };

int brushColor = dblClr(RGB(3, 5, 9));
int eraseColor = 0;

void fillSquare(Coordinate cursor, int clr) {
  drawRect(cursor, 4, 4, clr);
  drawRect(cursor, 3, 3, clr);
  drawRect(cursor, 2, 2, clr);
  drawRect(cursor, 1, 1, clr);
}

void updateBrushPosition(Coordinate *cursor) {
  if (keyHeld(U)) {
    cursor->y -= 1;
    if (keyHeld(L)) {
      cursor->x -= 1;
    } else if (keyHeld(R)) {
      cursor->x += 1;
    }
    for (volatile int x = 0; x < 10000; x++)
      ;
  } else if (keyHeld(D)) {
    cursor->y += 1;
    if (keyHeld(L)) {
      cursor->x -= 1;
    } else if (keyHeld(R)) {
      cursor->x += 1;
    }
    for (volatile int x = 0; x < 10000; x++)
      ;
  } else if (keyHeld(R)) {
    cursor->x += 1;
    if (keyHeld(U)) {
      cursor->y -= 1;
    } else if (keyHeld(D)) {
      cursor->y += 1;
    }
    for (volatile int x = 0; x < 10000; x++)
      ;
  } else if (keyHeld(L)) {
    cursor->x -= 1;
    if (keyHeld(U)) {
      cursor->y -= 1;
    } else if (keyHeld(D)) {
      cursor->y += 1;
    }
    for (volatile int x = 0; x < 10000; x++)
      ;
  }
}

int main() {
  DSPC = MODE3 | BG2;
  fillScreen(brushColor);

  enum MODE appState = DRAWING;

  Coordinate cursor = {0, 0};

  while (1) {
    updateKeys();
    VBLANK();
    switch (appState) {
    case (DRAWING):
      // input START sets appState to COLOR

      // cursor = diagonalInputHandler(cursor);
      if (keyUp(A) && keyUp(B)) {
        fillSquare(cursor, brushColor);
      }

      updateBrushPosition(&cursor);

      if (keyHeld(A)) {
        fillSquare(cursor, eraseColor);
      } else if (keyHeld(B)) {
        fillSquare(cursor, brushColor);
      } else {
        fillSquare(cursor, eraseColor);
      }
      break;
    case (COLOR):
      break;
    case (SHAPE):
      break;
    }
  }

  //  drawRect({8, 8}, 4, 32, color);

  return 0;
}
/*
case (COLOR):
switch (colorMode) {
// each colorMode individual color value change reflected in test square
// white regions of test square to be drawn as RGB15
case (RED):

while (colorMode == RED && appState == COLOR) {
u8 clr5 = 0;
if (keyDown(UP)) {
// if clr5 < 31, clr5++, wait- use key repeater to repeat every half
// second?
}
if (keyDown(DOWN)) {

// if clr5 > 0, clr5 --
}
if (keyDown(RIGHT)) {
// colorMode = green
}
if (keyDown(RT)) {
appState = SHAPE;
}

// up/down cycle red value ranging 0 to 31
// whitish cursor, moves 2 pixels at a time upwards, 64 px tall
// right, mode = GREEN
//
case (GREEN):
// up/down cycle green value
// left: red mode, right: blue mode
break;

case (BLUE):
// up/down cycle blue value
// left: green mode
break;
}
break;

case (SHAPE):
switch (brushShape) {
// only options here are left/right to toggle shapes for brushShape
case (SQUARE):
break;
case (TRIANGLE):
break;
case (CIRCLE):
break;
case (HEXAGON):
break;
}



  //  enum COLOR_SELECT colorMode = RED;
  //  enum SHAPE_SELECT brushShape = SQUARE;
  */
