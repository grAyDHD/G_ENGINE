#include "engine.h"

u16 keyCache;
u16 prevKeyCache;

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
    // UPDATE_KEYS(); // Update key states
    key_poll();

    switch (currentState) {
    case IDLE:
      if (key_hit(A)) { // A was just pressed
        FREQ_1 = 0xCF88;
        //      PLAY();
        currentState = PLAY_ONCE;
      } else if (key_is_down(B)) { // B was pressed
                                   // CONTINUOUS();
                                   // PLAY();
        FREQ_1 = 0x8088;
        currentState = CONTINUOUS;
      }
      break;

    case PLAY_ONCE:
      if (key_released(A)) { // A was released
        currentState = IDLE;
      }
      break;

    case CONTINUOUS:
      if (key_released(B)) { // B was released
        // TIMED();
        // PLAY(); // Play once on release
        FREQ_1 = 0xC0FF;
        currentState = SWITCH_TO_TIMED;
      }
      break;

    case SWITCH_TO_TIMED:
      if (key_released(A)) { // Ensure A is released before going back to IDLE
        currentState = IDLE;
      }
      break;
    }

    prevKeyCache = keyCache; // Store the current state as the previous state
  }

  return 0;
}
/*
case IDLE:
if (INPUT(A) && !(prevKeyCache & A)) { // A was just pressed
  FREQ = 0xCF88;
  //      PLAY();
  currentState = PLAY_ONCE;
} else if (INPUT(B)) { // B was pressed
                       // CONTINUOUS();
                       // PLAY();
  FREQ = 0x8088;
  currentState = CONTINUOUS;
}
break;

case PLAY_ONCE:
if (!(INPUT(A))) { // A was released
  currentState = IDLE;
}
break;

case CONTINUOUS:
if (!(INPUT(B))) { // B was released
  // TIMED();
  // PLAY(); // Play once on release
  FREQ = 0xC0FF;
  currentState = SWITCH_TO_TIMED;
}
break;

case SWITCH_TO_TIMED:
if (!(INPUT(A))) { // Ensure A is released before going back to IDLE
  currentState = IDLE;
}
break;
*/
