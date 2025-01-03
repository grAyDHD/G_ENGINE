#include "graphics/draw.h"
#include "graphics/video.h"

int main() {
  DSPC = MODE3 | BG2;
  int t = 0;
  GREEN_SWAP = 1;

  int shiftSpeed = 1;

  while (1) {

    for (int x = 0; x < 240; x++) {
      VBLANK();
      int color;
      for (int y = 0; y < 160; y++) {
        color = (x ^ y);
        // color = ((x << 4) | (x >> 4)) ^ y; // bars
        //  plotPixel(x, y, RGB(color, color << 1, color << 2));
        //  plotPixel(x, y, RGB(color, color, color ^ t));

        // plotPixel(x, y, RGB(color, color << 1, color << 2));
        // plotPixel(x, y, RGB(color, color >> 1, color << 1));
        // plotPixel(x, y, RGB(color * 255, 0, 255 - (color * 255)));
        plotPixel(x, y,
                  RGB(color & 0x1F, (color >> t) & 0x1F, (color ^ t) & 0x1F));

        /*
        // Create a subtle warping effect with slow changes over time
        int warpX = ((x + (t << 1)) ^ (x >> 1)) & 0xFF; // Gradual shift on x
        int warpY = ((y + (t >> 1)) ^ (y >> 1)) & 0xFF; // Gradual shift on y

        // Combine the warped x and y values for a smooth transition
        //        color = (warpX ^ warpY) & 0xFF;
        color = (warpX * warpY) ^ (warpX + warpY);

        // Apply the color with small variations based on the warp pattern
        int r = (color + (warpX >> 3)) & 0x1F;
        int g = (color + (warpY >> 4)) & 0x1F;
        int b = ((color >> 2) + (t >> 7)) & 0x1F;

        // Plot the pixel with the calculated color
        // plotPixel(x, y, RGB(r, g, b));
        plotPixel(x, y, RGB(r, g, b ^ t));
        color = ((x >> 3) ^ (y >> 3)) & 0x1F;
        color = ((x << 4) | (x >> 4)) ^ y;
        color = ((x >> 3) ^ (y >> 3)) & 1;
        color = (x ^ y);

        color = (x << 2) * (y >> 2) ^ (x + y);
*/
      }
    };
    t += shiftSpeed;
  }

  return 0;
}
