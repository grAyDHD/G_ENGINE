#include "gfx.h"

//refine bitwise logic?


int main() {
    DSPR = MODE3 | BG2;

    unsigned char currentState[SCREEN_WIDTH] = {0};
    unsigned char nextState[SCREEN_WIDTH] = {0};

    currentState[120] = 1; 

    for (int y = 0; y < SCREEN_HEIGHT; y++) { 
        // Plot the current state pixels
        for (int x = 0; x < SCREEN_WIDTH; x++) { 
            if (currentState[x] == 1) {
                plotPixel(x, y, 0x1F1F, VRAM);
            } else {
                plotPixel(x, y, 0x03A0, VRAM);
            }
        }

        // Calculate the next state
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            // Handle boundary conditions
            unsigned char left = (x == 0) ? 0 : currentState[x - 1];
            unsigned char center = currentState[x];
            unsigned char right = (x == SCREEN_WIDTH - 1) ? 0 : currentState[x + 1];

            // Rule 30 logic: next state is determined by left ^ (center || right)
            nextState[x] = left ^ (center || right);
        }

        // Copy nextState to currentState
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            currentState[x] = nextState[x];
        }
    }

    return 0;
}
