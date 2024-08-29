#include <dmg.h>
#include <gfx.h>
#include <input.h>
#include <typedefs.h>

#define UPDATE_KEY_CACHE() (prevKeyCache = keyCache);

u16 keyCache;
u16 prevKeyCache;

enum TetraState { ZERO, ONE, TWO, THREE, FOUR };
enum ToneState { NORMAL, STACCATO, LEGATO };
enum SoundState { IDLE, PLAY_NOTE };

int main() {
  ENABLE_SOUND();
  DMG_STEREO_OUTPUT = 0xFF77;
  DSPC = MODE3 | BG2;

  ENV = 0xC083;

  enum TetraState currentMode = ZERO;
  enum SoundState currentSoundState = IDLE;

  while (1) {
    UPDATE_KEYS();
    switch (currentSoundState) {
    case IDLE:
      // this logic seems tough to scale...

      if (INPUT(LS)) {

      } else if (INPUT(RS)) {

      } else if (INPUT(A)) {

      } else if (INPUT(B)) {
        FREQ = 0xCF88;
        currentSoundState = PLAY_NOTE;
      }
      break;
    case PLAY_NOTE:
      // hmmmmmm........ no fucking clue.
      if (!(INPUT(B))) {
        currentSoundState = IDLE;
      }
      break;
    }

    switch (currentMode) {
    case ZERO:
      break;

    case ONE:
      break;
    case TWO:
      break;
    case THREE:
      break;
    case FOUR:
      break;
    }
    UPDATE_KEY_CACHE(); // Store the current state as the previous state
  }
  return 0;
}
/*
if (INPUT(LS)) {
// FREQ = 0xCF88;
// PLAY();
// prevKeyCache &= ~(RS | A | B);
} else if (INPUT(RS)) {
// FREQ = 0xCD48;
// PLAY();
// prevKeyCache &= ~(A | B);
} else if (INPUT(A)) {
// FREQ = 0xCC93;
// PLAY();
// prevKeyCache &= ~B;
*/
