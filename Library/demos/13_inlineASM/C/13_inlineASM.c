#include "graphics/draw.h"
#include "graphics/video.h"
#include "input/in.h"

typedef struct {
  int size;              // 4 byte
  u16 color;             // 2 byte
  Coordinate coordinate; // 8 byte, {int, int}
} Square;

int colors[] = {RGB(31, 0, 0), RGB(0, 31, 0), RGB(0, 0, 31), RGB(31, 31, 0),
                RGB(31, 0, 31)};

int colorCount = sizeof(colors) / sizeof(colors[0]);
int currentColorIndex = 0;

// ASSEMBLY FUNCTION
void changeSquareSize(Square *square) {
  __asm__ __volatile__("ldr r1, [%0]\n"
                       "add r1, r1, #1\n"
                       "str r1, [%0]\n"
                       :
                       : "r"(square)
                       : "r1", "memory");
};
extern void increaseSquareSize(Square *square);
extern void decreaseSquareSize(Square *square);
//
int main() {
  DSPC = MODE3 | BG2;
  Square square = {10, colors[0], {120, 80}};

  while (1) {
    updateKeys();
    if (keyTapped(B)) {
      drawRect(square.coordinate, square.size, square.size, 0x0000);
      decreaseSquareSize(&square);
    } else if (keyTapped(A)) {
      drawRect(square.coordinate, square.size, square.size, 0x0000);
      increaseSquareSize(&square);
    } else if (keyTapped(RT | LT)) {
      changeSquareSize(&square);
    }
    drawRect(square.coordinate, square.size, square.size, square.color);
  }
  return 0;
}
