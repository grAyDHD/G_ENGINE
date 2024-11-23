#include "../include/drw.h"
#include "../include/state.h"
#include "../include/types.h"
#include "draw.h"
#include "gfx.h"

void clearScreen(Brush brush) {
  fillScreen(dblClr(brush.eraserColor));
  saveToBrushCache(brush);
}

void drawGradientsGUI(enum COLOR colorSelect, Brush brush) {
  Coordinate origin = {12, 56};

  drawRect(origin, 4, 4, RGB(31, 0, 0));
  if (colorSelect == RED) {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 31, 31));
    origin.x--;
    origin.y--;
  } else {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 0, 0));
    origin.x--;
    origin.y--;
  }
  origin.x += 12;

  drawRect(origin, 4, 4, RGB(0, 31, 0));
  if (colorSelect == GREEN) {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 31, 31));
    origin.x--;
    origin.y--;
  } else {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 0, 0));
    origin.x--;
    origin.y--;
  }
  origin.x += 12;

  drawRect(origin, 4, 4, RGB(0, 0, 31));
  if (colorSelect == BLUE) {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 31, 31));
    origin.x--;
    origin.y--;
  } else {
    origin.x++;
    origin.y++;
    drawRect(origin, 2, 2, RGB(31, 0, 0));
    origin.x--;
    origin.y--;
  }

  // draw gradient bar levels
  origin.x = 12; // y = 56
  for (int x = 0; x < 4; x++) {
    origin.y -= 6;
    if (brush.gradientScaleR >= x) {
      drawRect(origin, 4, 2, 0);
    } else {
      drawRect(origin, 4, 2, RGB(20, 20, 30));
    }
  }

  origin.x += 12;
  origin.y = 56; // y = 56
  for (int x = 0; x < 4; x++) {
    origin.y -= 6;
    if (brush.gradientScaleG >= x) {
      drawRect(origin, 4, 2, 0);
    } else {
      drawRect(origin, 4, 2, RGB(20, 20, 30));
    }
  }
  origin.x += 12;

  origin.x += 12;
  origin.y = 56; // y = 56
  for (int x = 0; x < 4; x++) {
    origin.y -= 6;
    if (brush.gradientScaleB >= x) {
      drawRect(origin, 4, 2, 0);
    } else {
      drawRect(origin, 4, 2, RGB(20, 20, 30));
    }
  }
}

void fillCircle(int x, int y, int radius, unsigned short color) {
  int r = radius;
  int s = 0;
  int decisionOver2 =
      1 - r; // Decision variable for the midpoint circle algorithm

  // Iterate until the step crosses the radius
  while (r >= s) {

    // Draw horizontal lines between mirrored points at each step
    // Top half
    drawHorizontalLine(x - r, x + r, y + s, color); // Line at y + s
    drawHorizontalLine(x - s, x + s, y + r, color); // Line at y + r

    // Bottom half
    drawHorizontalLine(x - r, x + r, y - s, color); // Line at y - s
    drawHorizontalLine(x - s, x + s, y - r, color); // Line at y - r
    //

    // Update s and adjust the decision variable
    s++;
    if (decisionOver2 <= 0) {
      // Midpoint is within the circle
      decisionOver2 += 2 * s + 1;
    } else {
      // Midpoint is outside the circle, reduce r
      r--;
      decisionOver2 += 2 * (s - r) + 1;
    }
  }
}

// Helper function to draw a horizontal line from x_start to x_end at a specific
// y
void drawHorizontalLine(int x_start, int x_end, int y, unsigned short color) {
  for (int x = x_start; x <= x_end; x++) {
    plotPixel(x, y, color); // Plot each pixel on the horizontal line
  }
}

void drawVerticalLine(int y_start, int y_end, int x, unsigned short color) {
  for (int y = y_start; y <= y_end; y++) {
    plotPixel(x, y, color); // Plot each pixel on the vertical line
  }
}

void drawColorPreview(Brush brush, int color) {
  Coordinate origin;

  // draw demo eraser
  origin.x = 10;
  origin.y = 48;
  drawRect(origin, 5, 5, brush.eraserColor);

  // draw demo brush
  origin.x = 18;
  drawRect(origin, 5, 5, brush.color);

  // draw demo RGB value
  origin.x = 40;
  drawRect(origin, 5, 5, color);
}

void drawColorSliders(enum COLOR colorSelect, int red, int green, int blue) {
  Coordinate origin;

  // RED slider
  origin.x = 8;
  origin.y = 8;
  if (colorSelect == RED) {
    drawRect(origin, 8, 34, RGB(31, 31, 31));
    for (int x = 0; x < 8; x++) {
      plotPixel(origin.x + x, origin.y + red, RGB(20, 20, 20));
      plotPixel(origin.x + x, origin.y + red + 2, RGB(14, 14, 12));
    }
  } else {
    drawRect(origin, 8, 34, RGB(0, 0, 0));
  }

  origin.x = 9;
  origin.y = 9;
  for (int x = 0; x < 32; x++) {
    drawRect(origin, 6, 1, RGB(x, 0, 0));
    origin.y++;
  }

  // GREEN slider
  origin.x = 20;
  origin.y = 8;
  if (colorSelect == GREEN) {
    drawRect(origin, 8, 34, RGB(31, 31, 31));
    for (int x = 0; x < 8; x++) {
      plotPixel(origin.x + x, origin.y + green, RGB(20, 20, 20));
      plotPixel(origin.x + x, origin.y + green + 2, RGB(14, 14, 12));
    }
  } else {
    drawRect(origin, 8, 34, RGB(0, 0, 0));
  }

  origin.x = 21;
  origin.y = 9;
  for (int x = 0; x < 32; x++) {
    drawRect(origin, 6, 1, RGB(0, x, 0));
    origin.y++;
  }

  // BLUE slider
  origin.x = 32;
  origin.y = 8;
  if (colorSelect == BLUE) {
    drawRect(origin, 8, 34, RGB(31, 31, 31));
    for (int x = 0; x < 8; x++) {
      plotPixel(origin.x + x, origin.y + blue, RGB(20, 20, 20));
      plotPixel(origin.x + x, origin.y + blue + 2, RGB(14, 14, 12));
    }
  } else {
    drawRect(origin, 8, 34, RGB(0, 0, 0));
  }

  origin.x = 33;
  origin.y = 9;
  for (int x = 0; x < 32; x++) {
    drawRect(origin, 6, 1, RGB(0, 0, x));
    origin.y++;
  }
}
