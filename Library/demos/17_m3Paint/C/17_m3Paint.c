#include "engine.h"
#include "gfx.h"
#include "in.h"
#include "typedefs.h"

#define VRAM_SCREEN_END 0x06012C00
#define CURSOR_CACHE ((u16)VRAM_SCREEN_END)

enum MODE { DRAWING = 0, COLOR, SHAPE } MODE;
enum COLOR_SELECT { RED = 0, GREEN, BLUE };
enum SHAPE_SELECT { SQUARE, TRIANGLE, CIRCLE, HEXAGON };

u16 pixelCache[4][4];
int brushColor = 0;
int eraseColor = dblClr(RGB(3, 5, 9));

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
  fillScreen(eraseColor);
  enum MODE appState = DRAWING;
  Coordinate cursor = {0, 0};

  saveToCursorCache(cursor);

  while (1) {
    updateKeys();
    VBLANK();

    switch (appState) {
    case (DRAWING):
      // input START sets appState to COLOR

      if (keyDown(ST)) {
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
      break;
    case (SHAPE):
      break;
    }
  }

  return 0;
}
