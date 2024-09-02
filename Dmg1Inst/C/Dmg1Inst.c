#include <dmg.h>
#include <gfx.h>
#include <input.h>
#include <typedefs.h>

#define PRESS_ONCE(k) ((prevKeyCache & k) && !(keyCache & k))

u16 keyCache;
u16 prevKeyCache = 0;
// b natural: 0xC6F7  ?? 0xC511, 0xC762
u16 NOTES[5][4] = {
    {0xC416, 0xC4B5, 0xC563,
     0xC5CE}, // C Eb G Bb  d: 0xC484 e: 0xC4E5, g: 0xC563
    {0xC2C7, 0xC3DA, 0xC484,
     0xC511}, // G B D F     g a b d 0xC2C7, 0xC358, 0xC3DD, 0xC484
              //
    {0xC4B5, 0xC563, 0xC5CE,
     0xC642}, // Eb G Bb D   e f a b 0xC4E5, 0xC511, 0xC563, 0xC5ED
    {0xC511, 0xC589, 0xC60B,
     0XC65B}, // F Ab C Eb   f g a c 0xC511, 0xC563, 0xC5AC, 0xC60B
    {0xC5ED, 0xC642, 0xC689,
     0xC6C4}}; // B D F Ab  g# a b d 0xC589, 0xC5AC, 0xC5ED, 0xC642
// 2048 - 131072/hz
enum ChordState { ZERO, ONE, TWO, THREE, FOUR };
enum KeyState { CMAJOR, CMINOR };

enum ToneState { NORMAL, STACCATO, LEGATO };
enum SoundState { IDLE, PLAY_NOTE };

enum ChordState updateChordState(enum ChordState chordState);

void playFrequency(enum ChordState chordState);

int main() {
  ENABLE_SOUND();
  DMG_STEREO_OUTPUT = 0xFF77;
  ENV = 0xC083;

  DSPC = MODE3 | BG2;
  enum ChordState chordState = ZERO;

  while (1) {
    UPDATE_KEYS();

    chordState = updateChordState(chordState);
    playFrequency(chordState);
    prevKeyCache = keyCache;
    waitVBLANK();
  }
  return 0;
}

// hold select for directions to change tone, hold start to to change note
// duration, hold both to set sweep/volume presets
enum ChordState updateChordState(enum ChordState chordState) {
  if (INPUT(ST)) {

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
    if (INPUT(U)) {
      ENV = (ENV & 0xFFC0) | 0x03;
      FREQ |= 0x8000;
    } else if (INPUT(D)) {
      ENV = (ENV & 0xFFC0) | (0x03C);
      FREQ |= 0x8000;
    } else if (INPUT(L)) {
      ENV = (ENV & 0xFFC0) | (0x3B);
      FREQ |= 0x8000;
    } else if (INPUT(R)) {
      ENV = (ENV & 0xFFC0) | (0x3F);
      FREQ |= 0x8000;
    }

  } else if (PRESS_ONCE(U)) {
    if (chordState == THREE) {
      chordState = ZERO;
    } else {
      chordState = THREE;
    }

  } else if (PRESS_ONCE(D)) {
    if (chordState == ONE) {
      chordState = ZERO;
    } else {
      chordState = ONE;
    }

  } else if (PRESS_ONCE(L)) {
    if (chordState == TWO) {
      chordState = ZERO;
    } else {
      chordState = TWO;
    }

  } else if (PRESS_ONCE(R)) {
    if (chordState == FOUR) {
      chordState = ZERO;
    } else {
      chordState = FOUR;
    }
  }

  return chordState;
}

void playFrequency(enum ChordState chordState) {
  switch (chordState) {
  case ZERO:
    if (PRESS_ONCE(LS)) {
      FREQ = NOTES[0][3];
    } else if (PRESS_ONCE(RS)) {
      FREQ = NOTES[0][2];
    } else if (PRESS_ONCE(A)) {
      FREQ = NOTES[0][1];
    } else if (PRESS_ONCE(B)) {
      FREQ = NOTES[0][0];
    }
    break;
  case ONE:
    if (PRESS_ONCE(LS)) {
      FREQ = NOTES[1][3];
    } else if (PRESS_ONCE(RS)) {
      FREQ = NOTES[1][2];
    } else if (PRESS_ONCE(A)) {
      FREQ = NOTES[1][1];
    } else if (PRESS_ONCE(B)) {
      FREQ = NOTES[1][0];
    }
    break;
  case TWO:
    if (PRESS_ONCE(LS)) {
      FREQ = NOTES[2][3];
    } else if (PRESS_ONCE(RS)) {
      FREQ = NOTES[2][2];
    } else if (PRESS_ONCE(A)) {
      FREQ = NOTES[2][1];
    } else if (PRESS_ONCE(B)) {
      FREQ = NOTES[2][0];
    }
    break;
  case THREE:
    if (PRESS_ONCE(LS)) {
      FREQ = NOTES[3][3];
    } else if (PRESS_ONCE(RS)) {
      FREQ = NOTES[3][2];
    } else if (PRESS_ONCE(A)) {
      FREQ = NOTES[3][1];
    } else if (PRESS_ONCE(B)) {
      FREQ = NOTES[3][0];
    }
    break;
  case FOUR:
    if (PRESS_ONCE(LS)) {
      FREQ = NOTES[4][3];
    } else if (PRESS_ONCE(RS)) {
      FREQ = NOTES[4][2];
    } else if (PRESS_ONCE(A)) {
      FREQ = NOTES[4][1];
    } else if (PRESS_ONCE(B)) {
      FREQ = NOTES[4][0];
    }
    break;
  }
}
