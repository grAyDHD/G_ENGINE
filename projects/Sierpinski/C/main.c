#include "engine.h"

// refine bitwise logic?

int main() {
  DSPC = MODE3 | BG2;

  unsigned char currentState[SW] = {0};
  unsigned char nextState[SW] = {0};

  currentState[120] = 1;

  for (int y = 0; y < SH; y++) {
    // Plot the current state pixels
    for (int x = 0; x < SH; x++) {
      if (currentState[x] == 1) {
        plotPixel(x, y, 0x1F1F);
      } else {
        plotPixel(x, y, 0x03A0);
      }
    }

    // Calculate the next state
    for (int x = 0; x < SW; x++) {
      // Handle boundary conditions
      unsigned char left = (x == 0) ? 0 : currentState[x - 1];
      unsigned char center = currentState[x];
      unsigned char right = (x == SW - 1) ? 0 : currentState[x + 1];

      // Rule 30 logic: next state is determined by left ^ (center || right)
      nextState[x] = left ^ (center || right);
    }

    // Copy nextState to currentState
    for (int x = 0; x < SW; x++) {
      currentState[x] = nextState[x];
    }
  }

  return 0;
}
