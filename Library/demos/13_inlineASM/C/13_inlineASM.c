#include "engine.h"

u16 keyCache = 0;
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
  key_poll();

  while (1) {
    if (key_held(B)) {
      drawRect(square.coordinate, square.size, square.size, 0x0000);
      decreaseSquareSize(&square);
    } else if (key_held(A)) {
      drawRect(square.coordinate, square.size, square.size, 0x0000);
      increaseSquareSize(&square);
    } else if (key_held(RT | LT)) {
      changeSquareSize(&square);
    }
    drawRect(square.coordinate, square.size, square.size, square.color);
  }
  return 0;
}
