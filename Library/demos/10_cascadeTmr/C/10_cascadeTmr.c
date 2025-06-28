#include "core/timer.h"
#include "graphics/draw.h"
#include "graphics/video.h"

int main() {
  DSPC = MODE3 | BG2;
  Coordinate rectXY = {0, 0};

  // Overflow every ~1 second:
  // 0x4000 ticks @ FREQ_1024
  TIMER[0].count = -0x4000;
  TIMER[0].control = TM_ENABLE | TM_FREQ_1024;

  TIMER[1].count = 0;
  TIMER[1].control = TM_ENABLE | TM_CASCADE;

  int time = TIMER[1].count;

  while (1) {
    // if check will pass one time whenever timer 1 increments
    if (time != TIMER[1].count) {
      time = TIMER[1].count;

      // clear rectangle
      drawRect(rectXY, 10, 10, RGB(0, 0, 0));

      // update rectangle position
      rectXY.x += 10;

      // re draw rectangle at new postion
      drawRect(rectXY, 10, 10, RGB(10, 1, 17));
    }
  }

  return 0;
}
