#include "draw.h"
#include "gfx.h"

enum MODE { DRAWING = 0, PAUSE, UNPAUSE, COLOR, SHAPE } MODE;
enum COLOR_SELECT { RED = 0, GREEN, BLUE };
enum SHAPE_SELECT { SQUARE, TRIANGLE, CIRCLE, HEXAGON };

// have u16 to represent color, perform bitwise operations to mask in each color
// mode ensure max in each color is 31 and min is 0

u16 color = 0x0000;

// red   0x0000 0000 0001 1111
// blue  0x0000 0011 1110 0000
// green 0x0111 1100 0000 0000

int main() {
  DSPC = MODE3 | BG2;
  // up/down cycle blue value
  fillScreen(dblClr(RGB(3, 5, 9)));

  enum MODE appState = DRAWING;
  enum COLOR_SELECT colorMode = RED;
  enum SHAPE_SELECT brushShape = SQUARE;

  while (1) {
    switch (appState) {
    case (DRAWING):
      // input START sets appState to PAUSE
      break;
    case (PAUSE):
      // wait briefly, set up for COLOR state
      // LT for COLOR mode, RT for SHAPE mode
      break;
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
        }
        // up/down cycle red value ranging 0 to 31
        // whitish cursor, moves 2 pixels at a time upwards, 64 px tall
        // right, mode = GREEN
        break;

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

      break;
    case (UNPAUSE):
      // here
      break;
    }
  }

  //  drawRect({8, 8}, 4, 32, color);

  return 0;
}
