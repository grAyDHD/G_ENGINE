/////////////Type Definitions/////////////
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef volatile unsigned char vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned long vu32;

typedef const unsigned char const_u8;
typedef const unsigned short const_u16;
typedef const unsigned long const_u32;

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;

typedef unsigned char byte;
typedef unsigned short hword;
typedef unsigned long word;
typedef void (*fp)(
    void); // this is the definition you will find in gba.h.  It is just

// INPUT
#define KEYS (*(volatile u16 *)0x4000130)
#define INPUT(k) (!(keyCache & (k)))
#define UPDATE_KEYS() (keyCache = KEYS)
#define A (1 << 0)
#define B (1 << 1)
#define SL (1 << 2)
#define ST (1 << 3)
#define R (1 << 4)
#define L (1 << 5)
#define U (1 << 6)
#define D (1 << 7)
#define RS (1 << 8)
#define LS (1 << 9)

u16 keyCache;
u16 prevKeyCache;

#define DMG_SOUND_SYSTEM *(volatile u16 *)0x04000084
#define ENABLE_SOUND() (DMG_SOUND_SYSTEM |= 0x0080)
#define DISABLE_SOUND() (DMG_SOUND_SYSTEM &= 0xFF7F);
#define DMG_STEREO_OUTPUT *(volatile u16 *)0x04000080
#define SOUND1INIT 0x8000 // makes the sound restart

#define DMG_1 *(vu32 *)0x4000060 // sound 1
#define SWEEP *(vu16 *)0x4000060 //
#define ENV *(vu16 *)0x4000062   //
#define FREQ *(vu16 *)0x4000064  //
#define REG_SGBIAS *(vu16 *)0x4000088
#define REG_SOUNDBIAS *(vu16 *)0x4000088 // bit rate+sound bias

#define DSPC *(u32 *)0x4000000
#define VRAM (u16 *)0x6000000
#define MODE3 (u16)0x400
#define BG2 (u16)0x003

u16 note;
u16 delta, u, sweepshifts = 2, sweepdir = 1, sweeptime = 7, cur = 6;
u16 envinit = 0xf, envdir = 0, envsteptime = 7, waveduty = 2, soundlength = 1;
u16 loopmode = 0, sfreq = 0x400, resamp = 1;

int main(void) {
  int wait = 0, f = 0;
  DSPC = MODE3 | BG2;
  // for (wait = 90; wait; wait--)
  // WaitForVblank(); // wait a bit
  // init();            // clear the screen
  // turn on sound circuit
  // REG_SOUNDCNT_X = 0x80;
  ENABLE_SOUND();

  // full volume, enable sound 1 to left and right
  // REG_SOUNDCNT_L = 0x1177;

  DMG_STEREO_OUTPUT = 0x1177;
  // Overall output ratio - Full
  // REG_SOUNDCNT_H = 2;

  while (1) {
    // check keys - u is cleared when all keys had been released
    u = (KEYS != 0x3ff);
    if (u) {
      wait++;
    } else {
      f = 0;
    } // wait for key auto-repeat

    if ((~KEYS & 0x3ff && !f) || (wait > 5000)) {

      if (!f) {
        wait = -20000;
      } else {
        wait = 0;
      };
      f = 1; // enable key auto-repeat

      if (~KEYS & A) // play the sound
      {
        SWEEP = (sweeptime << 4) + (sweepdir << 3) + sweepshifts;
        ENV = (envinit << 12) + (envdir << 11) + (envsteptime << 8) +
              (waveduty << 6) + soundlength;
        FREQ = SOUND1INIT + (loopmode << 14) + sfreq;
      }

      // move cursor
      if (~KEYS & (D | U)) {
        //   Print(0, cur, " ");
        if (~KEYS & D && cur < 17)
          cur++; // move cursor down
        if (~KEYS & U && cur > 6)
          cur--; // move cursor up
                 //        Print(0, cur, ">");
      }

      // adjust parameters values
      if (~KEYS & (L | R)) {
        switch (cur) {

        case 6: // sweep shifts
          if (~KEYS & L && sweepshifts > 0)
            sweepshifts--;
          if (~KEYS & R && sweepshifts < 7)
            sweepshifts++;
          SWEEP = (SWEEP & 0xfff8) + sweepshifts;
          break;

        case 7:          // sweep dir
          sweepdir ^= 1; // flip the bit
          SWEEP = (SWEEP & 0xfff7) + (sweepdir << 3);
          break;

        case 8: // sweep time
          if (~KEYS & L && sweeptime > 0)
            sweeptime--;
          if (~KEYS & R && sweeptime < 7)
            sweeptime++;
          SWEEP = (SWEEP & 0xff0f) + (sweeptime << 4);
          break;

        case 9: // sound len
          if (~KEYS & L && soundlength > 0)
            soundlength--;
          if (~KEYS & R && soundlength < 63)
            soundlength++;
          break;

        case 10: // wave duty
          if (~KEYS & L && waveduty > 0)
            waveduty--;
          if (~KEYS & R && waveduty < 3)
            waveduty++;
          ENV = (ENV & 0xff3f) + (waveduty << 6);
          break;

        case 11: // envellope step time
          if (~KEYS & L && envsteptime > 0)
            envsteptime--;
          if (~KEYS & R && envsteptime < 7)
            envsteptime++;
          ENV = (ENV & 0xf8ff) + (envsteptime << 8);
          break;

        case 12:       // envellope direction
          envdir ^= 1; // flip bit
          ENV = (ENV & 0xf7ff) + (envdir << 11);
          break;

        case 13: // envellope init volume
          if (~KEYS & L && envinit > 0)
            envinit--;
          if (~KEYS & R && envinit < 15)
            envinit++;
          ENV = (ENV & 0x0fff) + (envinit << 12);
          break;

        case 14: // frequency
          if (~KEYS & L && sfreq > 0)
            sfreq -= 16;
          if (~KEYS & R && sfreq < 2047)
            sfreq += 16;
          FREQ = (FREQ & 0xf800) + sfreq;
          break;

        case 15: // loop mode
          loopmode ^= 1;
          break;

        case 17: // resampling freq
          if (~KEYS & L && resamp > 0)
            resamp--;
          if (~KEYS & R && resamp < 3)
            resamp++;
          REG_SOUNDBIAS = (resamp << 14) + (REG_SOUNDBIAS & 0x3fff);
          break;
        }

        // ShowValues(); // display parameter values
      }
    }
  }
}
