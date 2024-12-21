#include "gfx/draw.h"
#include "gfx/gfx.h"

#define COLOR(r, g, b) (((r) & 0x1F) << 10 | ((g) & 0x1F) << 5 | ((b) & 0x1F))

#define SW 240
#define SH 160

//---Display control---//
#define DSPC *(u32 *)0x4000000
//---Video buffer---
#define VRAM (u16 *)0x6000000

//---Modes---//

// void plotPixel(int x, int y, u16 clr) { ((u16 *)VRAM)[y * SW + x] = clr; }

void drawCircle(int x, int y, int radius, unsigned short color) {
  int r = radius;
  int s = 0;
  int decisionOver2 =
      1 - r; // Decision criterion divided by 2 evaluated at r=r, s=0

  while (r >= s) {
    //    for (volatile int z = 0; z < 1000; z++)
    //    ;

    // Draw the eight octants
    plotPixel(x + r, y + s, color); // Octant 1
    plotPixel(x + s, y + r, color); // Octant 2
    plotPixel(x - s, y + r, color); // Octant 3
    plotPixel(x - r, y + s, color); // Octant 4
    plotPixel(x - r, y - s, color); // Octant 5
    plotPixel(x - s, y - r, color); // Octant 6
    plotPixel(x + s, y - r, color); // Octant 7
    plotPixel(x + r, y - s, color); // Octant 8

    /*

    plotPixel(x + r, y + s, COLOR(r, 0, 0)); // Octant 1
    plotPixel(x + s, y + r, COLOR(r, r, 0)); // Octant 2
    plotPixel(x - s, y + r, COLOR(r, 0, r)); // Octant 3
    plotPixel(x - r, y + s, COLOR(0, r, 0)); // Octant 4
    plotPixel(x - r, y - s, COLOR(0, 0, r)); // Octant 5
    plotPixel(x - s, y - r, COLOR(0, r, r)); // Octant 6
    plotPixel(x + s, y - r, COLOR(0, 0, r)); // Octant 7
    plotPixel(x + r, y - s, COLOR(r, r, r)); // Octant 8
     */

    s++;
    if (decisionOver2 <= 0) {
      decisionOver2 += 2 * s + 1; // Move down in y-direction
    } else {
      r--;
      decisionOver2 += 2 * (s - r) + 1; // Move down in both x and y
    }
  }
}

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
      if (r % 3 == 0) {
        drawCircle(50, 50, r, COLOR(R + r, G, B));
      } else if (r % 3 == 1) {
        drawCircle(190, 50, r, COLOR(R, G + r, B));
      } else if (r % 3 == 2) {
        drawCircle(120, 110, r, COLOR(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      if (r % 3 == 0) {
        drawCircle(40, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 25, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(200, 120, r, COLOR(R, G, B + r));
      }
    }
    // WAIT
    for (volatile int w = 0; w < 1500000; w++)
      ;

    // Overlap A
    for (int r = 10; r < maxR; r++) {
      if (r % 3 == 0) {
        drawCircle(120, 110, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(50, 50, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(190, 50, r, COLOR(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      if (r % 3 == 0) {
        drawCircle(200, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(40, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(120, 25, r, COLOR(R, G, B + r));
      }
    }

    // WAIT
    for (volatile int w = 0; w < 5000000; w++)
      ;

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
      if (r % 3 == 0) {
        drawCircle(50, 50, r, COLOR(R + r, G, B));
      } else if (r % 3 == 1) {
        drawCircle(190, 50, r, COLOR(R, G + r, B));
      } else if (r % 3 == 2) {
        drawCircle(120, 110, r, COLOR(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      if (r % 3 == 0) {
        drawCircle(40, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 25, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(200, 120, r, COLOR(R, G, B + r));
      }
    }

    // WAIT
    for (volatile int w = 0; w < 1500000; w++)
      ;

    // Overlap B
    for (int r = 10; r < maxR; r++) {
      if (r % 3 == 0) {
        drawCircle(190, 50, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 110, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(50, 50, r, COLOR(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      if (r % 3 == 0) {
        drawCircle(120, 25, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(200, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(40, 120, r, COLOR(R, G, B + r));
      }
    }

    // WAIT
    for (volatile int w = 0; w < 5000000; w++)
      ;

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
      if (r % 3 == 0) {
        drawCircle(190, 50, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 110, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(50, 50, r, COLOR(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      if (r % 3 == 0) {
        drawCircle(120, 25, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(200, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(40, 120, r, COLOR(R, G, B + r));
      }
    }

    // WAIT
    for (volatile int w = 0; w < 1500000; w++)
      ;

    for (int r = 10; r < maxR; r++) {
      if (r % 3 == 0) {
        drawCircle(120, 110, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(50, 50, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(190, 50, r, COLOR(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      if (r % 3 == 0) {
        drawCircle(200, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(40, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(120, 25, r, COLOR(R, G, B + r));
      }
    }

    // WAIT
    for (volatile int w = 0; w < 5000000; w++)
      ;

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
      if (r % 3 == 0) {
        drawCircle(50, 50, r, COLOR(R + r, G, B));
      } else if (r % 3 == 1) {
        drawCircle(190, 50, r, COLOR(R, G + r, B));
      } else if (r % 3 == 2) {
        drawCircle(120, 110, r, COLOR(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      if (r % 3 == 0) {
        drawCircle(40, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 25, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(200, 120, r, COLOR(R, G, B + r));
      }
    }

    // WAIT
    for (volatile int w = 0; w < 1500000; w++)
      ;

    // Overlap A
    for (int r = 10; r < maxR; r++) {
      if (r % 3 == 0) {
        drawCircle(120, 110, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(50, 50, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(190, 50, r, COLOR(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      if (r % 3 == 0) {
        drawCircle(200, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(40, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(120, 25, r, COLOR(R, G, B + r));
      }
    }

    // WAIT
    for (volatile int w = 0; w < 1500000; w++)
      ;

    // Overlap B
    for (int r = 10; r < maxR; r++) {
      if (r % 3 == 0) {
        drawCircle(190, 50, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(120, 110, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(50, 50, r, COLOR(R, G, B + r));
      }
    }
    for (int r = 3; r < 25; r++) {
      if (r % 3 == 0) {
        drawCircle(120, 25, r, COLOR(R, G, B + r));
      } else if (r % 3 == 1) {
        drawCircle(200, 120, r, COLOR(R, G, B + r));
      } else if (r % 3 == 2) {
        drawCircle(40, 120, r, COLOR(R, G, B + r));
      }
    }

    // WAIT
    for (volatile int w = 0; w < 5000000; w++)
      ;

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
