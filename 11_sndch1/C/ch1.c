#define SOUND_SYSTEM *(volatile short *)0x04000084
#define ENABLE_SOUND()                                                         \
  (SOUND_SYSTEM |= 0x0080) // Activates sound hardware for all channels
#define DISABLE_SOUND() (SOUND_SYSTEM &= ~0x0080) // Deactivates sound hardware

#define SND_CH1 *(volatile short *)0x04000060
#define SND_CH2 *(volatile short *)0x04000068

#define OFFSET(channel, offset) (*(volatile u16 *)((u16 *)(&channel) + offset))

#define SWEEP_RANGE(channel, range)                                            \
  (channel = (channel & ~0x07) | ((8 - (range)) & 0x07))

#define SWEEP_TIME(channel, time)                                              \
  (channel = (channel & ~(0x07 << 4)) | (((8 - (time)) & 0x07) << 4))
// 0 is up 1 is down
#define SWEEP_DIRECTION(channel, direction)                                    \
  (channel = (channel & ~(1 << 3)) | (direction))

#define SWEEP_OFF(channel) (channel = channel & ~0x7F)

// Envelope macros (volume swell/diminish)
#define VOL_SWELL(channel) (OFFSET(channel, 1) |= (1 << 11))
#define VOL_DIM(channel) (OFFSET(channel, 1) &= ~(1 << 11))

// Volume change rate and tone setup
#define V_CHANGE_RATE(channel, rate)                                           \
  (OFFSET(channel, 1) =                                                        \
       (OFFSET(channel, 1) & ~(0x07 << 8)) | ((rate & 0x07) << 8))

#define TONE(channel, tone)                                                    \
  (OFFSET(channel, 1) =                                                        \
       (OFFSET(channel, 1) & ~(0x03 << 6)) | ((tone & 0x03) << 6))

#define START_VOLUME(channel, volume)                                          \
  (OFFSET(channel, 1) =                                                        \
       (OFFSET(channel, 1) & ~(0x0F << 12)) | ((volume & 0x0F) << 12))

#define NOTE_LENGTH(channel, length)                                           \
  (OFFSET(channel, 1) = (OFFSET(channel, 1) & ~0x1F) | ((length) & 0x1F))

// Continuous vs Timed mode
#define CONTINUOUS(channel)                                                    \
  (OFFSET(channel, 2) &= ~(1 << 14)) // Continuous mode (bit 14 off)
#define TIMED(channel)                                                         \
  (OFFSET(channel, 2) |= (1 << 14)) // Timed mode (bit 14 on)

#define SND_RESET(channel) (OFFSET(channel, 2) |= (1 << 15)) // Reset sound

// Note frequency setup
#define NOTE(channel, note, octave)                                            \
  (OFFSET(channel, 2) =                                                        \
       (OFFSET(channel, 2) & ~0x07FF) | ((note >> (octave)) & 0x07FF))

#define PLAY(REG_)
// Predefined note values
#define C 0x777
#define CS 0x70D
#define D 0x6AD
#define DS 0x653
#define E 0x5F4
#define F 0x5A0
#define FS 0x54E
#define G 0x502
#define GS 0x4B9
#define A 0x475
#define AS 0x435
#define B 0x3F9

void setup_timer() {
  REG_TM0CNT = 0; // Stop the timer
  REG_TM0CNT_L =
      65536 - 27342;     // Set timer to trigger every ~60Hz (roughly 16.67ms)
  REG_TM0CNT_H = 0x00C1; // Start timer, 1024 prescaler, enable interrupt
  REG_IE |= 0x0008;      // Enable timer 0 interrupt
  REG_TM0CNT |= 0x0080;  // Enable timer 0
}

void timer_isr() {
  // Load the next note or perform other music-related tasks
  // Acknowledge the interrupt
  REG_IF = 0x0008;
}

int main() {
  ENABLE_SOUND();
  SWEEP_RANGE(SND_CH1, 2);
  SWEEP_TIME(SND_CH1, 4);
  SWEEP_DIRECTION(SND_CH1, 1); // down

  VOL_SWELL(SND_CH1);
  V_CHANGE_RATE(SND_CH1, 4);
  START_VOLUME(SND_CH1, 12);

  TONE(SND_CH1, 2);        // wave cycle 3, 0 indexed 0-3
  NOTE_LENGTH(SND_CH1, 15) // set note length (0-31)

  NOTE(SND_CH1, NOTE_C, 0);
  CONTINUOUS(SND_CH1);
  SND_RESET(SND_CH1);

  VOL_DIM(SND_CH2);          // volume decreases
  V_CHANGE_RATE(SND_CH2, 3); // set volume change rate, 0-7
  START_VOLUME(SND_CH2, 10); // 0-15

  TONE(SND_CH2, 3);
  NOTE_LENGTH(SND_CH2, 20); // 0-31

  NOTE(SND_CH2, NOTE_G, 1);
  CONTINUOUS(SND_CH2);
  SND_RESET(SND_CH2);

  int melody[] = {C, D, E, F, G};
  int harmony[] = {G, F, E, D, C};
  int num_notes = 5;

  for (int i = 0; i < num_notes; i++) {
    // Set frequency for melody
    NOTE(SND_CH1, melody[i], 1);
    SND_RESET(SND_CH1);

    NOTE(SND_CH2, harmony[i], 1);
    SND_RESET(SND_CH2)
  }

  while (1) {
    // Main game loop: handle input, game logic, rendering, etc
  }

  return 0;
}
