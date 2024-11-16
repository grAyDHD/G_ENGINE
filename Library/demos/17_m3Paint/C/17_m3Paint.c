#include "../include/f.h"
#include "engine.h"

int main() {
  DSPC = MODE3 | BG2;

  enum MODE appState = DRAWING;
  enum COLOR colorSelect = RED;
  int red = 0, green = 0, blue = 0;
  Coordinate origin = {0, 0};
  Brush brush;

  brush.size = 4;
  brush.color = RGB(0, 0, 0);
  brush.eraserColor = RGB(31, 31, 31);
  brush.shape = SQUARE;
  brush.coordinates.x = 0;
  brush.coordinates.y = 0;

  fillScreen(dblClr(brush.eraserColor));

  saveToBrushCache(brush);

  while (1) {
    updateKeys();
    VBLANK();

    switch (appState) {
    case (DRAWING):

      if (keyTapped(ST)) {
        appState = COLORS;
        saveToGUICache();
        origin.x = 0;
        origin.y = 0;
        for (int x = 32; x > 0; x--) {
          drawRect(origin, 2 * x, 2 * x, RGB(10, 10, 5));
          origin.x++;
          origin.y++;
        }
      }

      // if (keyDown(LT) && keyDown(RT) && keyDown(A) && keyDown(B)) {
      if (keyDown(LT) && keyDown(RT)) {
        fillScreen(dblClr(brush.eraserColor));
        saveToBrushCache(brush);
        draw(brush);
      }

      if (keyUp(A) && keyUp(B)) {
        if (keyWasDown(U) || keyWasDown(D) || keyWasDown(L) || keyWasDown(R)) {
          restoreFromBrushCache(brush);
          updateBrushPosition(&brush.coordinates);
          saveToBrushCache(brush);
          draw(brush);
        }
      } else if (keyHeld(A)) {
        draw(brush);
        saveToBrushCache(brush);
        if (keyWasDown(U) || keyWasDown(D) || keyWasDown(L) || keyWasDown(R)) {
          restoreFromBrushCache(brush);
          updateBrushPosition(&brush.coordinates);
          draw(brush);
          saveToBrushCache(brush);
        }
      } else if (keyHeld(B)) {
        erase(brush);
        saveToBrushCache(brush);
        draw(brush);
        if (keyWasDown(U) || keyWasDown(D) || keyWasDown(L) || keyWasDown(R)) {
          restoreFromBrushCache(brush);
          updateBrushPosition(&brush.coordinates);
          erase(brush);
          saveToBrushCache(brush);
          draw(brush);
        }
      }

      break;
    case (COLORS):
      // draw 3 rects, 8 px wide, 4 px spacing, 36 px tall
      if (keyTapped(RT)) {
        appState = SHAPES;
        origin.x = 0;
        origin.y = 0;

        for (int x = 32; x > 0; x--) {
          drawRect(origin, 2 * x, 2 * x, RGB(10, 0, 12));
          origin.x++;
          origin.y++;
        }

        // drawShapeGUI, drawing rectangle size of brush.size
        origin.x = 32;
        origin.y = 32;
        if (brush.shape == SQUARE) {
          for (int x = brush.size; x > 0; x--) {
            drawRect(origin, x, x, brush.color);
            origin.x++;
            origin.y++;
          }
        } else if (brush.shape == CIRCLE) {
          drawCircle(origin.x - brush.size, origin.y - brush.size, brush.size,
                     brush.color);
        }

        break;
      }

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

      if (keyTapped(L)) {
        if (colorSelect == GREEN) {
          colorSelect = RED;
        } else if (colorSelect == BLUE) {
          colorSelect = GREEN;
        }
      } else if (keyTapped(R)) {
        if (colorSelect == GREEN) {
          colorSelect = BLUE;
        } else if (colorSelect == RED) {
          colorSelect = GREEN;
        }

      } else if (keyDown(D)) {
        switch (colorSelect) {
        case (RED):
          if (red < 31) {
            red++;
          }
          break;
        case (GREEN):
          if (green < 31) {
            green++;
          }
          break;
        case (BLUE):
          if (blue < 31) {
            blue++;
          }
          break;
        }

      } else if (keyDown(U)) {
        switch (colorSelect) {
        case (RED):
          if (red > 0) {
            red--;
          }
          break;
        case (GREEN):
          if (green > 0) {
            green--;
          }
          break;
        case (BLUE):
          if (blue > 0) {
            blue--;
          }
          break;
        }
      }

      if (keyTapped(A)) {
        brush.color = RGB(red, green, blue);
      }
      if (keyTapped(B)) {
        brush.eraserColor = RGB(red, green, blue);
      }

      origin.x = 10;
      origin.y = 48;
      drawRect(origin, 5, 5, brush.eraserColor);

      origin.x = 18;
      drawRect(origin, 5, 5, brush.color);

      origin.x = 40;
      drawRect(origin, 5, 5, RGB(red, green, blue));

      if (keyTapped(ST)) {
        appState = DRAWING;
        restoreFromGUICache();
      }
      for (volatile int x = 0; x < 20000; x++)
        ;
      break;
    case (SHAPES):

      origin.x = 0;
      origin.y = 0;
      if (keyTapped(LT)) {
        drawRect(origin, 64, 64, RGB(10, 0, 12));
        appState = COLORS;
        for (int x = 32; x > 0; x--) {
          drawRect(origin, 2 * x, 2 * x, RGB(10, 10, 5));
          origin.x++;
          origin.y++;
        }
      } else if (keyTapped(ST)) {
        restoreFromGUICache();
        appState = DRAWING;
      }

      origin.x = 32;
      origin.y = 32;
      if (keyTapped(U)) {
        if (brush.shape == SQUARE && brush.size < 32) {
          brush.size++;
          drawRect(origin, brush.size, brush.size, brush.color);
        } else if (brush.shape == CIRCLE && brush.size < 16) {
          brush.size++;
          drawCircle(origin.x - brush.size, origin.x - brush.size, brush.size,
                     brush.color);
        }
      } else if (keyTapped(D)) {
        if (brush.shape == SQUARE && brush.size > 2) {

          drawRect(origin, brush.size, brush.size, brush.eraserColor);
          brush.size--;
        } else if (brush.shape == CIRCLE && brush.size > 2) {
          drawCircle(origin.x - brush.size, origin.y - brush.size, brush.size,
                     brush.eraserColor);
          brush.size--;
        }
      } else if (keyTapped(R)) {
        brush.shape = CIRCLE;
        if (brush.size > 16) {
          brush.size = 16;
        }
      } else if (keyTapped(L)) {
        brush.shape = SQUARE;
      }

      break;
    }
  }

  return 0;
}
