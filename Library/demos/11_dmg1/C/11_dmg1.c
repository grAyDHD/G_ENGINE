#include "audio/dmg.h"
#include "graphics/video.h"
#include "input/in.h"

enum SoundState { IDLE, PLAY_ONCE, CONTINUOUS, SWITCH_TO_TIMED };

int main() {
  DSPC = MODE3 | BG2;
  ENABLE_SOUND();
  DMG_STEREO_OUTPUT = 0xFF77;

  SWEEP_RANGE(0);
  SWEEP_TIME(0);
  SWEEP_DIRECTION(0);

  ENV_1 = 0xC780;

  enum SoundState currentState = IDLE;

  while (1) {
    updateKeys();

    switch (currentState) {
    case IDLE:
      if (keyTapped(A)) { // A was just pressed
        FREQ_1 = 0xCF88;
        //      PLAY();
        currentState = PLAY_ONCE;
      } else if (keyDown(B)) { // B was pressed
                               // CONTINUOUS();
                               // PLAY();
        FREQ_1 = 0x8088;
        currentState = CONTINUOUS;
      }
      break;

    case PLAY_ONCE:
      if (keyReleased(A)) { // A was released
        currentState = IDLE;
      }
      break;

    case CONTINUOUS:
      if (keyReleased(B)) { // B was released
        // TIMED();
        // PLAY(); // Play once on release
        FREQ_1 = 0xC0FF;
        currentState = SWITCH_TO_TIMED;
      }
      break;

    case SWITCH_TO_TIMED:
      if (keyReleased(A)) { // Ensure A is released before going back to IDLE
        currentState = IDLE;
      }
      break;
    }
  }

  return 0;
}
