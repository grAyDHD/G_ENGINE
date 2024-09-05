#include <gfx.h>
#include <input.h>
#include <typedefs.h>

#define PRESS_ONCE(k) ((prevKeyCache & k) && !(keyCache & k))

u16 keyCache;
u16 prevKeyCache = 0;

u16 NOTES[5][4] = {{0xC416, 0xC4B5, 0xC563, 0xC5CE},
                   {0xC2C7, 0xC3DA, 0xC484, 0xC511},
                   {0xC4B5, 0xC563, 0xC5CE, 0xC642},
                   {0xC511, 0xC589, 0xC60B, 0XC65B},
                   {0xC5ED, 0xC642, 0xC689, 0xC6C4}};

enum ChordState { ZERO, ONE, TWO, THREE, FOUR };

#define DEBOUNCE_TIME 1000
static int debounceCounter = 0;

#define DMG_SOUND_SYSTEM *(volatile u16 *)0x04000084
#define ENABLE_SOUND() (DMG_SOUND_SYSTEM |= 0x0080)
#define DMG_STEREO_OUTPUT *(volatile u16 *)0x04000080
#define ENV *(volatile u16 *)0x04000062
#define FREQ *(volatile u16 *)0x04000064

#define LENGTH_VERY_SHORT 0x3F
#define LENGTH_SHORT 0x2A
#define LENGTH_LONG 0x15
#define LENGTH_VERY_LONG 0x00

// New global variable to store the current note length
u16 currentNoteLength = LENGTH_VERY_LONG;

enum ChordState updateChordState(enum ChordState chordState);
void playFrequency(enum ChordState chordState);

int main() {
  ENABLE_SOUND();
  DMG_STEREO_OUTPUT = 0xFF77;
  ENV = 0xC000 |
        currentNoteLength; // Set initial envelope with the default note length
  FREQ |= 0x8000;

  DSPC = MODE3 | BG2;
  enum ChordState chordState = ZERO;

  while (1) {
    while (debounceCounter > 0) {
      debounceCounter--;
    }
    UPDATE_KEYS();

    chordState = updateChordState(chordState);
    playFrequency(chordState);
    prevKeyCache = keyCache;
    waitVBLANK();
  }
  return 0;
}

enum ChordState updateChordState(enum ChordState chordState) {
  if (INPUT(ST) || INPUT(SL)) {
    if (debounceCounter == 0) {
      if (INPUT(ST)) {
        // Handle wave duty changes
        if (INPUT(U)) {
          ENV = (ENV & 0xFF3F) | (0x00 << 6);
        } else if (INPUT(D)) {
          ENV = (ENV & 0xFF3F) | (0x01 << 6);
        } else if (INPUT(L)) {
          ENV = (ENV & 0xFF3F) | (0x02 << 6);
        } else if (INPUT(R)) {
          ENV = (ENV & 0xFF3F) | (0x03 << 6);
        }
      } else if (INPUT(SL)) {
        // Handle sound length changes
        if (PRESS_ONCE(U)) {
          currentNoteLength = LENGTH_VERY_SHORT;
        } else if (PRESS_ONCE(D)) {
          currentNoteLength = LENGTH_VERY_LONG;
        } else if (PRESS_ONCE(L)) {
          currentNoteLength = LENGTH_SHORT;
        } else if (PRESS_ONCE(R)) {
          currentNoteLength = LENGTH_LONG;
        }
        ENV = (ENV & 0xFFC0) | currentNoteLength;
        FREQ |= 0x8000; // Reset sound length
      }
      debounceCounter = DEBOUNCE_TIME;
    }
  } else if (PRESS_ONCE(U)) {
    chordState = (chordState == THREE) ? ZERO : THREE;
  } else if (PRESS_ONCE(D)) {
    chordState = (chordState == ONE) ? ZERO : ONE;
  } else if (PRESS_ONCE(L)) {
    chordState = (chordState == TWO) ? ZERO : TWO;
  } else if (PRESS_ONCE(R)) {
    chordState = (chordState == FOUR) ? ZERO : FOUR;
  }

  return chordState;
}

void playFrequency(enum ChordState chordState) {
  u16 note = 0;
  if (PRESS_ONCE(LS)) {
    note = NOTES[chordState][3];
  } else if (PRESS_ONCE(RS)) {
    note = NOTES[chordState][2];
  } else if (PRESS_ONCE(A)) {
    note = NOTES[chordState][1];
  } else if (PRESS_ONCE(B)) {
    note = NOTES[chordState][0];
  }

  if (note != 0) {
    ENV = (ENV & 0xFFC0) | currentNoteLength; // Apply current note length
    FREQ = note | 0x8000;                     // Set frequency and restart sound
  }
}
