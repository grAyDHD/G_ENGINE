#include <dmg.h>
#include <gfx.h>
#include <input.h>
#include <typedefs.h>
// how the fuck are sounds even playing what the fuck?
#define UPDATE_KEY_CACHE() (prevKeyCache = keyCache);

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

  ENV = 0xC780;

  enum SoundState currentState = IDLE;

  while (1) {
    UPDATE_KEYS(); // Update key states

    switch (currentState) {
    case IDLE:
      if (INPUT(A) && !(prevKeyCache & A)) { // A was just pressed
        FREQ = 0xCF88;
        currentState = PLAY_ONCE;
      } else if (INPUT(B)) { // B was pressed
        FREQ = 0x8088;
        currentState = CONTINUOUS;
      }
      break;

    case PLAY_ONCE:
      if (!(INPUT(A))) { // A was released
        currentState = IDLE;
      }
      break;
      // ahhhhh I think I know the issue, I need to write actual macros for
      // setting FREQ/playing a note, as right now, the bits to initiate note
      // play back I believe are encoded in the value I am setting for FREQ.  I
      // remember now a few weeks ago writing this, I wasn't getting expected
      // behavior when setting the frequency THEN setting play, but rather
      // triggering them simultaneously.
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
    }

    UPDATE_KEY_CACHE(); // Store the current state as the previous state
  }

  return 0;
}
