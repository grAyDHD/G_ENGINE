#define DSPC *(u32 *)0x4000000
#define MODE3 (u16)0x400
#define BG2 (u16)0x003
#define VCOUNT (*(volatile unsigned short *)0x04000006)

#define KEYS (*(volatile unsigned short *)0x4000130)
#define INPUT(k) (!(keyCache & (k)))
#define UPDATE_KEYS() (keyCache = KEYS)
#define PRESS_ONCE(k) ((prevKeyCache & k) && !(keyCache & k))

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
extern unsigned short keyCache;
extern unsigned short prevKeyCache;

#define DMG_SOUND_SYSTEM *(volatile unsigned short *)0x04000084
#define ENABLE_SOUND() (DMG_SOUND_SYSTEM |= 0x0080)
#define DISABLE_SOUND() (DMG_SOUND_SYSTEM &= 0xFF7F);
#define DMG_STEREO_OUTPUT *(volatile unsigned short *)0x04000080

#define DMG_1 *(volatile unsigned short *)0x04000060
#define ENV *(volatile unsigned short *)0x04000062
#define FREQ *(volatile unsigned short *)0x04000064

enum ChordState { ZERO, ONE, TWO, THREE, FOUR };

enum ToneState { NORMAL, STACCATO, LEGATO };
enum SoundState { IDLE, PLAY_NOTE };
