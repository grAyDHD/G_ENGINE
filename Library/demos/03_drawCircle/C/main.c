#include "core/timer.h"
#include "graphics/draw.h"
#include "graphics/video.h"

int main() {
  DSPC = (MODE3 | BG2);

  int R = 0, G = 10, B = 20;
  int maxR = 45;

  R = 10;
  G = 0;
  B = 20;
  while (1) {

    // 1st iteration, no overlapping
    for (int r = 10; r < maxR; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(50, 50, r, RGB(R + r, G, B));
      } else if (r % 3 == 1) {
        drawCircle(190, 50, r, RGB(R, G + r, B));
      } else if (r % 3 == 2) {
        drawCircle(120, 110, r, RGB(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(40, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 25, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(200, 120, r, RGB(R, G, B + r));
      }
    }

    // WAIT
    simpleWait(1000);

    // Overlap A
    for (int r = 10; r < maxR; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(120, 110, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(50, 50, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(190, 50, r, RGB(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(200, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(40, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(120, 25, r, RGB(R, G, B + r));
      }
    }

    // WAIT
    simpleWait(1000);

    // CLEAR SCREEN top down
    for (int y = 0; y < 160; y++) {
      for (int x = 0; x < 240; x++) {
        for (volatile int w = 0; w < 5; w++)
          ;
        plotPixel(x, y, 0);
      }
    }

    // 2nd iteration, change color, no overlaps
    for (int r = 10; r < maxR; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(50, 50, r, RGB(R + r, G, B));
      } else if (r % 3 == 1) {
        drawCircle(190, 50, r, RGB(R, G + r, B));
      } else if (r % 3 == 2) {
        drawCircle(120, 110, r, RGB(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(40, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 25, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(200, 120, r, RGB(R, G, B + r));
      }
    }

    // WAIT
    simpleWait(1000);

    // Overlap B
    for (int r = 10; r < maxR; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(190, 50, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 110, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(50, 50, r, RGB(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(120, 25, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(200, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(40, 120, r, RGB(R, G, B + r));
      }
    }

    // WAIT
    simpleWait(1000);

    // CLEAR SCREEN left right
    for (int x = 0; x < 240; x++) {
      for (int y = 0; y < 160; y++) {
        for (volatile int w = 0; w < 5; w++)
          ;
        plotPixel(x, y, 0);
      }
    }

    // Overlap C (A+B overlaps)
    for (int r = 10; r < maxR; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(190, 50, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 110, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(50, 50, r, RGB(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(120, 25, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(200, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(40, 120, r, RGB(R, G, B + r));
      }
    }

    // WAIT
    simpleWait(1000);

    for (int r = 10; r < maxR; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(120, 110, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(50, 50, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(190, 50, r, RGB(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(200, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(40, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(120, 25, r, RGB(R, G, B + r));
      }
    }

    // WAIT
    simpleWait(1000);

    // CLEAR SCREEN down up
    for (int x = 240; x > 0; x--) {
      for (int y = 160; y > 0; y--) {
        for (volatile int w = 0; w < 5; w++)
          ;
        plotPixel(x, y, 0);
      }
    }

    // Final iteration, all overlaps
    for (int r = 10; r < maxR; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(50, 50, r, RGB(R + r, G, B));
      } else if (r % 3 == 1) {
        drawCircle(190, 50, r, RGB(R, G + r, B));
      } else if (r % 3 == 2) {
        drawCircle(120, 110, r, RGB(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(40, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 25, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(200, 120, r, RGB(R, G, B + r));
      }
    }

    // WAIT
    simpleWait(1000);

    // Overlap A
    for (int r = 10; r < maxR; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(120, 110, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(50, 50, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(190, 50, r, RGB(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(200, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(40, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(120, 25, r, RGB(R, G, B + r));
      }
    }

    // WAIT
    simpleWait(1000);

    // Overlap B
    for (int r = 10; r < maxR; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(190, 50, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 110, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(50, 50, r, RGB(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      simpleWait(10);
      if (r % 3 == 0) {
        drawCircle(120, 25, r, RGB(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(200, 120, r, RGB(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(40, 120, r, RGB(R, G, B + r));
      }
    }

    // WAIT
    simpleWait(1000);

    // CLEAR SCREEN
    for (int y = 160; y > 0; y--) {
      for (int x = 240; x > 0; x--) {
        for (volatile int w = 0; w < 5; w++)
          ;
        plotPixel(x, y, 0);
      }
    }
  }

  return 0;

  // approximate curve x^2+y^2=r^2
  // 2r = radius, or r << 2
  // start with center coords
  // lets go 120, 80
  //
}
