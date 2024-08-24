#include <dmg.h>
#include <draw.h>
#include <gfx.h>
#include <input.h>
#include <typedefs.h> //citrus

#define MEM_IO 0x04000000
#define REG_BASE MEM_IO

#define TMR0_DURATION *(volatile u16 *)(0x04000100)
#define TMR1_DURATION *(volatile u16 *)(REG_BASE + 0x0104)
#define TMR2_DURATION *(volatile u16 *)(REG_BASE + 0x0108)
#define TMR3_DURATION *(volatile u16 *)(REG_BASE + 0x010C)

#define TMR0_CONTROL *(volatile u16 *)(REG_BASE + 0x0102)
#define TMR1_CONTROL *(volatile u16 *)(REG_BASE + 0x0106)
#define TMR2_CONTROL *(volatile u16 *)(REG_BASE + 0x010A)
#define TMR3_CONTROL *(volatile u16 *)(REG_BASE + 0x010E)

#define TM_FREQ_1 0         //!< 1 cycle/tick (16.7 Mhz)
#define TM_FREQ_64 0x0001   //!< 64 cycles/tick (262 kHz)
#define TM_FREQ_256 0x0002  //!< 256 cycles/tick (66 kHz)
#define TM_FREQ_1024 0x0003 //!< 1024 cycles/tick (16 kHz)

#define TM_ENABLE 0x0080
#define TM_CASCADE 0x0004
#define TM_FREQ_MASK 0x0003

u16 keyCache;
u16 prevKeyCache;
u16 pee;

void func() {
  for (int poop = 0; poop > pee; poop++) {
  }
};

enum TimerState { STOPPED, PAUSED, RUNNING };
enum GameState { TITLE_SCREEN, TRANSITION, GAMEPLAY, SCORE_SCREEN };

void vid_vsync(void) {
  while (VCOUNT >= 160)
    ; // wait till VDraw
  while (VCOUNT < 160)
    ; // wait till VBlank
}

void initializeTimers_CS2(u16 tmA_Length, u16 tmB_Length) {
  TMR0_DURATION = tmA_Length; //-0x4000 ~ 1 sec
  TMR2_DURATION = tmB_Length;

  TMR0_CONTROL = TM_ENABLE | TM_FREQ_1024;
  TMR2_CONTROL = TM_ENABLE | TM_FREQ_1024;

  TMR1_CONTROL = TM_ENABLE | TM_CASCADE;
  TMR3_CONTROL = TM_ENABLE | TM_CASCADE;
}

void initializeAudio() {
  ENABLE_SOUND();
  DMG_STEREO_OUTPUT = 0xFF77;
  ENV = 0xC780;
}

// timer A, music, timer B
// adjust timer A to play a melody
//  use timer B as an input buffer???
//  if btn press within buffer range, change gradient?
int main() {
  DSPC = MODE3 | BG2;
  initializeAudio();
  enum GameState gameState = TITLE_SCREEN;

  Coordinate rectCrnr = {0, 0};
  int width = SW;
  int height = SH;

  while (1) {
    vid_vsync();
    switch (gameState) {
    case TITLE_SCREEN:
      UPDATE_KEYS();
      if (INPUT(ST)) {
        gameState = TRANSITION;
      }
      break;
    case TRANSITION:
      // draw rectangles- # =SH/2, or 80
      for (int r = 0; r < 80; r++) {
        drawRect(rectCrnr, width, height, COLOR(r, 1, 14), VRAM);
        rectCrnr.x++;
        rectCrnr.y++;
        width -= 2;
        height -= 2;
      }
      rectCrnr.x = (SW / 2 - 4);
      rectCrnr.y = (SH / 2 - 4);
      gameState = GAMEPLAY;
      break;
    case GAMEPLAY:
      drawRect(rectCrnr, 8, 8, COLOR(3, 7, 7), VRAM);
      break;
    case SCORE_SCREEN:
      break;
    }
    //    UPDATE_KEYS();
  }
  return 0;
}
/*
// Coordinate rectCrnr = {0, 0};

while (1) {
  vid_vsync();
  UPDATE_KEYS();
  switch (gameState) {

  case TITLE_SCREEN:

    if (INPUT(ST)) {

      gameState = TRANSITION;
    }

    break;

  case TRANSITION:

    gameState = GAMEPLAY;

    break;

  case GAMEPLAY:

    break;

  case SCORE_SCREEN:

    break;
  }
}
*/

// set-up berk font
//  tte_init_se(0, BG_CBB(0) | BG_SBB(31), 1, 0, 0, &berkFont, se_drawg);
// tte_init_con();
// memcpy16(pal_bg_mem, berkPal, berkPalLen / 4);
/*
  enum TimerState currentState = STOPPED;

  switch (currentState) {
  case RUNNING:
    if (INPUT(A) && !(prevKeyCache & A)) { // A was just pressed
                                           //    FREQ = 0xCF88;
      //      PLAY();
        //      set cascade for tm0
      currentState = PAUSED;
    } else if (INPUT(B)) { // B was pressed
                           // CONTINUOUS();
                           // PLAY();
        // stop tm0
//      FREQ = 0x8088;
      currentState = STOPPED;
    }
    break;

  case PAUSED:
    if (!(INPUT(A))) {
        // A was released

        currentState = RUNNING;
    }
    break;

  case STOPPED:
    if (!(INPUT(B))) { // B was released
      // TIMED();
      // PLAY(); // Play once on release
      //FREQ = 0xC0FF;
      //currentState = RUNNING;
    }
    break;

  //case SWITCH_TO_TIMED:
    if (!(INPUT(A))) { // Ensure A is released before going back to IDLE
      currentState = IDLE;
    }
    break;
  }

  prevKeyCache = keyCache; // Store the current state as the previous state
*
*
*
*
void checkInput() {
  UPDATE_KEYS();
  if ((KEYS & A) == 0) {
    TMR0_CONTROL ^= TM_ENABLE;
  }
  if ((KEYS & B) == 0) {
    TMR0_CONTROL ^= TM_CASCADE;
  }
}



void tmr_test() {
  TMR0_DURATION = -0x4000;     // 0x4000 ticks till overflow
  TMR0_CONTROL = TM_FREQ_1024; // we're using the 1024 cycle timer
  // cascade into tm3
  TMR1_CONTROL = TM_ENABLE | TM_CASCADE;
  u32 sec = -1;
  while (1) {
    vid_vsync();
    if (TMR1_DURATION != sec) {
      FREQ = 0xCF88;
      sec = TMR1_DURATION;

      //      tte_printf("#{es;P:24,60}%02d:%02d:%02d", sec / 3600, (sec % 3600)
      //      / 60,
      //   sec % 60);
    }
  }
}
*/
// REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;
//  tmr_test();
