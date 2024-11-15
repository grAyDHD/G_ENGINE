#include "draw.h"
#include "engine.h"
#include "gfx.h"
#include "in.h"
#include "typedefs.h"

#define VRAM_SCREEN_END 0x06012C00
#define CURSOR_CACHE ((u16)VRAM_SCREEN_END)

enum MODE { DRAWING = 0, COLORS, SHAPES };
enum COLOR { RED = 0, GREEN, BLUE };
enum SHAPE { SQUARE, TRIANGLE, CIRCLE, HEXAGON };

u16 pixelCache[32][32];
u16 guiCache[64][64];
//
// future params: shape, brush_size
typedef struct {
  Coordinate coordinates;
  enum SHAPE shape;
  u16 size; // for now, max size of 32
  u16 color;
  u16 eraserColor;
} Brush;

void saveToBrushCache(Brush brush) {
  for (int x = 0; x < brush.size; x++) {
    for (int y = 0; y < brush.size; y++) {
      pixelCache[x][y] = ((u16 *)VRAM)[(brush.coordinates.y + y) * SW +
                                       (brush.coordinates.x + x)];
    }
  }
}

void saveToGUICache() {
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 64; y++) {
      guiCache[x][y] = ((u16 *)VRAM)[(y * 240) + x];
    }
  }
}

void restoreFromBrushCache(Brush brush) {
  for (int x = 0; x < brush.size; x++) {
    for (int y = 0; y < brush.size; y++) {
      plotPixel((brush.coordinates.x + x), (brush.coordinates.y + y),
                pixelCache[x][y]);
    }
  }
}

void restoreFromGUICache() {
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 64; y++) {
      plotPixel(x, y, guiCache[x][y]);
    }
  }
}

void draw(Brush brush) {
  for (int x = 0; x < brush.size / 2; x++) {
    Coordinate origin = {brush.coordinates.x + x, brush.coordinates.y + x};
    drawRect(origin, (brush.size - (2 * x)), (brush.size - (2 * x)),
             brush.color);
  }
}

void erase(Brush brush) {
  for (int x = 0; x < brush.size / 2; x++) {
    Coordinate origin = {brush.coordinates.x + x, brush.coordinates.y + x};
    drawRect(origin, (brush.size - (2 * x)), (brush.size - (2 * x)),
             brush.eraserColor);
  }
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

  enum MODE appState = DRAWING;
  enum COLOR colorSelect = RED;
  int red = 0, green = 0, blue = 0;
  Coordinate origin = {0, 0};
  Brush brush;

  brush.size = 4;
  brush.color = RGB(0, 0, 0);
  brush.eraserColor = RGB(31, 31, 31);
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

      // drawShapeGUI, drawing rectangle size of brush.size
      origin.x = 32;
      origin.y = 32;

      for (int x = brush.size; x > 0; x--) {
        drawRect(origin, x, x, brush.color);
        origin.x++;
        origin.y++;
      }
      if (keyTapped(U)) {
        if (brush.size < 32) {

          brush.size++;
        }
      } else if (keyTapped(D)) {
        if (brush.size > 2) {

          origin.x = 32;
          origin.y = 32;
          drawRect(origin, brush.size, brush.size, brush.eraserColor);

          brush.size--;
        }
      }
      //      drawRect(origin, brush.size, brush.size, brush.color);

      break;
    }
  }

  return 0;
}
