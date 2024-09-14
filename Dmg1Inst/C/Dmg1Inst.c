#include <gfx.h>
#include <input.h>
#include <typedefs.h>

#define SOUND_SYSTEM *(volatile u16 *)0x04000084
#define ENABLE_SOUND() (SOUND_SYSTEM |= 0x0080)
#define DMG_STEREO_OUTPUT *(volatile u16 *)0x04000080
#define ENV *(volatile u16 *)0x04000062
#define FREQ *(volatile u16 *)0x04000064

#define PRESS_ONCE(k) ((prevKeyCache & k) && !(keyCache & k))

u16 keyCache;
u16 prevKeyCache = 0;

enum ChordState { ZERO, ONE, TWO, THREE, FOUR };
u16 NOTES[5][4] = {{0x0416, 0x04B5, 0x0563, 0x05CE},
                   {0x02C7, 0x03DA, 0x0484, 0x0511},
                   {0x04B5, 0x0563, 0x05CE, 0x0642},
                   {0x0511, 0x0589, 0x060B, 0X065B},
                   {0x05ED, 0x0642, 0x0689, 0x06C4}};
u16 note = 0;

#define LENGTH_VERY_SHORT 0x3F
#define LENGTH_SHORT 0x2A
#define LENGTH_LONG 0x15
#define LENGTH_VERY_LONG 0x00
u16 currentNoteLength = LENGTH_VERY_SHORT;

int main() {
  ENABLE_SOUND();
  DMG_STEREO_OUTPUT = 0xFF77;
  ENV = 0xC000 | currentNoteLength;

  DSPC = MODE3 | BG2;
  enum ChordState chordState = ZERO;

  while (1) {
    UPDATE_KEYS();
    if (INPUT(ST) || INPUT(SL)) {
      if (INPUT(ST)) {
        // Handle wave duty changes
        if (PRESS_ONCE(U)) {
          ENV = (ENV & 0xFF3F) | (0x00 << 6);
        } else if (PRESS_ONCE(D)) {
          ENV = (ENV & 0xFF3F) | (0x01 << 6);
        } else if (PRESS_ONCE(L)) {
          ENV = (ENV & 0xFF3F) | (0x02 << 6);
        } else if (PRESS_ONCE(R)) {
          ENV = (ENV & 0xFF3F) | (0x03 << 6);
        }
      } else if (INPUT(SL)) {
        // Handle sound length changes
        if (PRESS_ONCE(U)) {
          currentNoteLength = LENGTH_VERY_SHORT;
          ENV = (ENV & 0xFFC0) | currentNoteLength;
          FREQ = 0x4000; // sets bit 15 to zero, timed mode
        } else if (PRESS_ONCE(D)) {
          currentNoteLength = LENGTH_VERY_LONG;
          ENV = (ENV & 0xFFC0) | currentNoteLength;
          FREQ = 0x4000; // sets bit 15 to zero, timed mode
        } else if (PRESS_ONCE(L)) {
          currentNoteLength = LENGTH_SHORT;
          ENV = (ENV & 0xFFC0) | currentNoteLength;
          FREQ = 0x4000; // sets bit 15 to zero, timed mode
        } else if (PRESS_ONCE(R)) {
          currentNoteLength = LENGTH_LONG;
          ENV = (ENV & 0xFFC0) | currentNoteLength;
          FREQ = 0x4000; // sets bit 15 to zero, timed mode
        }
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

    // play note on button press
    if (PRESS_ONCE(LS)) {
      note = NOTES[chordState][3];
      FREQ = note | 0xC000;
    } else if (PRESS_ONCE(RS)) {
      note = NOTES[chordState][2];
      FREQ = note | 0xC000;
    } else if (PRESS_ONCE(A)) {
      note = NOTES[chordState][1];
      FREQ = note | 0xC000;
    } else if (PRESS_ONCE(B)) {
      note = NOTES[chordState][0];
      FREQ = note | 0xC000;
    }

    prevKeyCache = keyCache;
    waitVBLANK();
  }

  return 0;
}

// ENV Register
// 0000 0000 0000 0000
// 0000 0000 0011 1111  note length
// 0000 0000 1100 0000 wave cycle
// 0000 0111 0000 0000 step time
// 0000 1000 0000 0000 step direction
// 1111 0000 0000 0000 initial volume
//
// 0100 4
// 0101 5
// 0110 6
// 0111 7
//
// 1000 8,
// 1001 9,
// 1010 A,
// 1011 B,
// 1100 C,
// 1101 D,
// 1110 E,
// 1111 F

// 0000 0111 1111 1111 frequency value
// 0011 1000 0000 0000 unused
// 0100 0000 0000 0000 1 = timed 0 = continuous
// 1000 0000 0000 0000 enable bit
