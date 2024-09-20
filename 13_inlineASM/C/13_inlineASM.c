#include <draw.h>
#include <gfx.h>
#include <input.h>
#include <typedefs.h>
u16 keyCache = 0;
typedef struct {
  int size;              // 4 byte
  u16 color;             // 2 byte
  Coordinate coordinate; // 8 byte, {int, int}
} Square;

int colors[] = {COLOR(31, 0, 0), COLOR(0, 31, 0), COLOR(0, 0, 31),
                COLOR(31, 31, 0), COLOR(31, 0, 31)};
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

// extern void changeSquareSize(Square *square);
int main() {
  DSPC = MODE3 | BG2;
  Square square = {10, colors[0], {120, 80}};
  while (1) {
    if (INPUT(B)) {
      changeSquareSize(&square);
    }
    drawRect(square.coordinate, square.size, square.size, square.color, VRAM);
    UPDATE_KEYS();
  }

  return 0;
}
