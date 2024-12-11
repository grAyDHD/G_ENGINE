#include "timer.h"

void simpleWait(int delay) {
  for (volatile int z = 0; z < (delay * 1000); z++)
    ;
}
