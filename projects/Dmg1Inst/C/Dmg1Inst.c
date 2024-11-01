#include "in.h"
#include <engine.h>

enum OctaveState { TWO = 0, THREE, FOUR, FIVE, SIX, SEVEN };
enum Position { FIRST, SECOND, THIRD, FOURTH };
enum RootNote { CC = 0, Db, DD, Eb, E, F, Gb, G, Ab, AA, Bb, BB };
enum Mode {
  IONIAN = 0,
  DORIAN,
  PHRYGIAN,
  LYDIAN,
  MIXOLYDIAN,
  AEOLIAN,
  LOCRIAN
};

const u8 MODE_INTERVALS[7][8] = {
    {0, 2, 4, 5, 7, 9, 11, 12}, // Ionian
    {0, 2, 3, 5, 7, 9, 10, 12}, // Dorian
    {0, 1, 3, 5, 7, 8, 10, 12}, // Phrygian
    {0, 2, 4, 6, 7, 9, 11, 12}, // Lydian
    {0, 2, 4, 5, 7, 9, 10, 12}, // Mixolydian
    {0, 2, 3, 5, 7, 8, 10, 12}, // Aeolian
    {0, 1, 3, 5, 6, 8, 10, 12}  // Locrian
};

const u16 NOTES[12][7] = {
    {0x2C, 0x416, 0x60B, 0x706, 0x782, 0x7C1, 0X7E0},  // c2-c5    0
    {0x9D, 0x44E, 0x627, 0x714, 0x789, 0x7C4, 0x7E2},  // c#2-c#5  1
    {0x107, 0x483, 0x642, 0x721, 0x790, 0x7C8, 0x7E4}, // d2-d5    2
    {0x16B, 0x4B5, 0x65B, 0x72D, 0x796, 0x7CB, 0x7E5}, // eb2-eb5  3
    {0x1CA, 0x4E5, 0x672, 0x739, 0x79C, 0x7CE, 0x7E7}, // e2-e5    4
    {0x223, 0x511, 0x689, 0x744, 0x7A2, 0x7D1, 0x7E8}, // f2-f5    5
    {0x277, 0x53C, 0x69E, 0x74F, 0x7A7, 0x7D3, 0x7E9}, // f#2-f#5  6
    {0x2C7, 0x563, 0x6B2, 0x759, 0x7AC, 0x7D6, 0x7Eb}, // g2-g5    7
    {0x312, 0x589, 0x6C4, 0x762, 0x7B1, 0x7D8, 0x7EC}, // g#2-g#5  8
    {0x358, 0x5AC, 0x6D6, 0x76B, 0x7B5, 0x7DA, 0x7ED}, // a2-a5    9
    {0x39B, 0x5CE, 0x6E7, 0x773, 0x7B9, 0x7DC, 0x7EE}, // bb2-bb5  10
    {0x3DA, 0x5ED, 0x6F7, 0x77B, 0x7BD, 0x7DE, 0x7EF}, // b2-b5    11
};

u16 note = 0;

enum OctaveState currentOctave = TWO;
enum Position currentPosition = FIRST;
enum RootNote currentRoot = Ab;
enum Mode currentMode = AEOLIAN;

void playNote(u16 noteFreq) {
  ENV_1 = 0xF0BF;
  FREQ_1 = noteFreq | 0x8000;
}

void playScaleDegree(int scaleDegree) {
  int interval = MODE_INTERVALS[currentMode][scaleDegree - 1];
  int noteOffset = currentRoot + interval;
  int octaveOffset = noteOffset / 12;
  int noteIndex = noteOffset % 12;
  int octave = currentOctave + octaveOffset;

  if (octave > SEVEN)
    octave = SEVEN;

  playNote(NOTES[noteIndex][octave]);
}

void updatePosition() {
  if (key_is_up(LT) && key_is_up(RT)) {
    currentPosition = FIRST;
  } else if (key_is_down(LT) && key_is_up(RT)) {
    currentPosition = SECOND;
  } else if (key_is_up(LT) && key_is_down(RT)) {
    currentPosition = THIRD;
  } else if (key_is_down(LT) && key_is_down(RT)) {
    currentPosition = FOURTH;
  }
}

void changeOctave() {
  if (key_hit(UP) && currentOctave < SEVEN) {
    currentOctave++;
  } else if (key_hit(DN) && currentOctave > TWO) {
    currentOctave--;
  }
}

void changeRoot() {
  if (key_is_down(ST) && key_is_up(SL) && key_hit(LT)) {
    currentRoot = (currentRoot + 5) % 12; // Equivalent to -7 mod 12
  } else if (key_is_down(ST) && key_is_up(SL) && key_hit(RT)) {
    currentRoot = (currentRoot + 7) % 12; // Move up a fifth
  }
  if (key_is_down(SL) && key_is_down(ST) && key_hit(LT)) {
    currentRoot = (currentRoot + 11) % 12;
  } else if (key_is_down(SL) && key_is_down(ST) && key_hit(RT)) {
    currentRoot = (currentRoot + 1) % 12;
  }
}

void changeMode() {
  if (key_is_down(SL) && key_is_up(ST) && key_hit(LT)) {
    currentMode = (currentMode + 6) % 7; // Previous mode
  } else if (key_is_down(SL) && key_is_up(ST) && key_hit(RT)) {
    currentMode = (currentMode + 1) % 7; // Next mode
  }
}

int main() {
  DSPC = MODE3 | BG2;
  ENABLE_SOUND();
  DMG_STEREO_OUTPUT = 0xFF77;
  FREQ_1 = CONTINUOUS();

  while (1) {
    VBLANK();
    key_poll();

    changeOctave();
    changeRoot();
    changeMode();
    if (key_is_up(B) && key_is_up(A) && ENV_1 > 0)
      ENV_1 = 0;

    if (key_is_up(ST) && key_is_up(SL))
      updatePosition();

    if (currentPosition == FIRST) {
      if (key_is_down(B) && ENV_1 == 0) {
        playScaleDegree(1);
      } else if (key_is_down(A) && ENV_1 == 0) {
        playScaleDegree(2);
      }
    } else if (currentPosition == SECOND) {
      if (key_is_down(B) && ENV_1 == 0) {
        playScaleDegree(3);
      } else if (key_is_down(A) && ENV_1 == 0) {
        playScaleDegree(4);
      }
    } else if (currentPosition == THIRD) {
      if (key_is_down(B) && ENV_1 == 0) {
        playScaleDegree(5);
      } else if (key_is_down(A) && ENV_1 == 0) {
        playScaleDegree(6);
      }
    } else if (currentPosition == FOURTH) {
      if (key_is_down(B) && ENV_1 == 0) {
        playScaleDegree(7);
      } else if (key_is_down(A) && ENV_1 == 0) {
        playScaleDegree(8);
      }
    }
  }

  return 0;
}
