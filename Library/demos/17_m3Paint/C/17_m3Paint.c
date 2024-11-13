#include "draw.h"
#include "engine.h"
#include "gfx.h"
#include "in.h"
#include "typedefs.h"

#define VRAM_SCREEN_END 0x06012C00
#define CURSOR_CACHE ((u16)VRAM_SCREEN_END)

enum MODE { DRAWING = 0, COLOR, SHAPE };
enum COLOR_SELECT { RED = 0, GREEN, BLUE };
enum SHAPE_SELECT { SQUARE, TRIANGLE, CIRCLE, HEXAGON };
u16 pixelCache[4][4];
int brushColor = 0;
int eraseColor = RGB(31, 31, 31);
//
// future params: shape, brush_size
void saveToCursorCache(Coordinate cursorPosition) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      pixelCache[x][y] =
          ((u16 *)VRAM)[(cursorPosition.y + y) * SW + (cursorPosition.x + x)];
    }
  }
}

void restoreFromCursorCache(Coordinate cursorPosition) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      plotPixel((cursorPosition.x + x), (cursorPosition.y + y),
                pixelCache[x][y]);
    }
  }
}

void fillSquare(Coordinate cursor, int clr) {
  drawRect(cursor, 4, 4, clr);
  drawRect(cursor, 3, 3, clr);
  drawRect(cursor, 2, 2, clr);
  drawRect(cursor, 1, 1, clr);
}

void updateBrushPosition(Coordinate *cursor) {
  if (keyHeld(U)) {
    cursor->y -= 1;
    if (keyHeld(L)) {
      cursor->x -= 1;
    } else if (keyHeld(R)) {
      cursor->x += 1;
    }
  } else if (keyHeld(D)) {
    cursor->y += 1;
    if (keyHeld(L)) {
      cursor->x -= 1;
    } else if (keyHeld(R)) {
      cursor->x += 1;
    }
  } else if (keyHeld(R)) {
    cursor->x += 1;
    if (keyHeld(U)) {
      cursor->y -= 1;
    } else if (keyHeld(D)) {
      cursor->y += 1;
    }
  } else if (keyHeld(L)) {
    cursor->x -= 1;
    if (keyHeld(U)) {
      cursor->y -= 1;
    }
  }
  for (volatile int x = 0; x < 10000; x++)
    ;
}

int main() {
  DSPC = MODE3 | BG2;
  fillScreen(dblClr(eraseColor));

  enum MODE appState = DRAWING;
  enum COLOR_SELECT colorSelect = RED;
  Coordinate cursor = {0, 0};
  Coordinate origin = {0, 0};

  int red = 0, green = 0, blue = 0;

  saveToCursorCache(cursor);

  while (1) {
    updateKeys();
    VBLANK();

    switch (appState) {
    case (DRAWING):

      if (keyDown(ST)) {
        appState = COLOR;

        origin.x = 0;
        origin.y = 0;
        for (int x = 32; x > 0; x--) {
          drawRect(origin, (2 * x), (2 * x), RGB(1, 20, 8));
          origin.x++;
          origin.y++;
        }
      }

      if (keyDown(LT) && keyDown(RT) && keyDown(A) && keyDown(B)) {
        fillScreen(eraseColor);
        saveToCursorCache(cursor);
        fillSquare(cursor, brushColor);
      }

      if (keyUp(A) && keyUp(B)) {
        if (keyWasDown(U) || keyWasDown(D) || keyWasDown(L) || keyWasDown(R)) {
          restoreFromCursorCache(cursor);
          updateBrushPosition(&cursor);
          saveToCursorCache(cursor);
          fillSquare(cursor, brushColor);
        }
      } else if (keyHeld(A)) {
        fillSquare(cursor, brushColor);
        saveToCursorCache(cursor);
        if (keyWasDown(U) || keyWasDown(D) || keyWasDown(L) || keyWasDown(R)) {
          restoreFromCursorCache(cursor);
          updateBrushPosition(&cursor);
          fillSquare(cursor, brushColor);
          saveToCursorCache(cursor);
        }
      } else if (keyHeld(B)) {
        fillSquare(cursor, eraseColor);
        saveToCursorCache(cursor);
        fillSquare(cursor, brushColor);
        if (keyWasDown(U) || keyWasDown(D) || keyWasDown(L) || keyWasDown(R)) {
          restoreFromCursorCache(cursor);
          updateBrushPosition(&cursor);
          fillSquare(cursor, eraseColor);
          saveToCursorCache(cursor);
          fillSquare(cursor, brushColor);
        }
      }

      break;
    case (COLOR):
      // draw 3 rects, 8 px wide, 4 px spacing, 36 px tall

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

      if (keyDown(L)) {
        if (colorSelect == GREEN) {
          colorSelect = RED;
        } else if (colorSelect == BLUE) {
          colorSelect = GREEN;
        }
      } else if (keyDown(R)) {
        if (colorSelect == GREEN) {
          colorSelect = BLUE;
        } else if (colorSelect == RED) {
          colorSelect = GREEN;
        }
      } else if (keyDown(U)) {

        switch (colorSelect) {
        case (RED):
          red++;
          break;
        case (GREEN):
          green++;
          break;
        case (BLUE):
          blue++;
          break;
        }
      } else if (keyDown(D)) {
        switch (colorSelect) {
        case (RED):
          red--;
          break;
        case (GREEN):
          green--;
          break;
        case (BLUE):
          blue--;
          break;
        }
      }

      if (keyTapped(A)) {
        brushColor = RGB(red, green, blue);
      }
      if (keyTapped(B)) {
        eraseColor = RGB(red, green, blue);
      }

      origin.x = 10;
      origin.y = 48;
      drawRect(origin, 5, 5, eraseColor);

      origin.x = 18;
      drawRect(origin, 5, 5, brushColor);

      origin.x = 40;
      drawRect(origin, 5, 5, RGB(red, green, blue));

      if (keyDown(ST) && keyDown(SL)) {
        appState = DRAWING;
      }
      for (volatile int x = 0; x < 20000; x++)
        ;
      break;
    case (SHAPE):
      break;
    }
  }

  return 0;
}
