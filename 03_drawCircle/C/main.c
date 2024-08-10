#include "engine.h"

void drawCircle(int x, int y, int radius, unsigned short color, unsigned short *vmem) {
    int r = radius;
    int s = 0;
    int decisionOver2 = 1 - r;  // Decision criterion divided by 2 evaluated at r=r, s=0

    while (r >= s) {
        // Draw the eight octants
        plotPixel(x + r, y + s, color, VRAM); // Octant 1
        plotPixel(x + s, y + r, color, VRAM); // Octant 2
        plotPixel(x - s, y + r, color, VRAM); // Octant 3
        plotPixel(x - r, y + s, color, VRAM); // Octant 4
        plotPixel(x - r, y - s, color, VRAM); // Octant 5
        plotPixel(x - s, y - r, color, VRAM); // Octant 6
        plotPixel(x + s, y - r, color, VRAM); // Octant 7
        plotPixel(x + r, y - s, color, VRAM); // Octant 8

        s++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * s + 1;  // Move down in y-direction
        } else {
            r--;
            decisionOver2 += 2 * (s - r) + 1;  // Move down in both x and y
        }
    }
}
int main() { 
    DSPC = (MODE3 | BG2);
    drawCircle(120, 80, 40, COLOR(23,2,14), VRAM);
    return 0;

    //approximate curve x^2+y^2=r^2
    //2r = radius, or r << 2
    //start with center coords
    //lets go 120, 80
    //
}
