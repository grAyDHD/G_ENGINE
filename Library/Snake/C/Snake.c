#include "Snake.h"
#include "engine.h"

typedef struct {
  Coordinate xy;
} Food;

int main() {
  DSPC = MODE3 | BG2;

  Snake player;
  Segment head = {.xy = {GW / 2, GH / 2}, .next = 0};
  player.head = &head;
  player.tail = &head;
  player.length = 1;
  player.direction = RIGHT;

  while (1) {
    clearSnake(&player);
    updateSnakePosition(&player);
    drawSnake(&player);

    for (volatile int t = 0; t < 50000; t++) {
      handleInput(&player);
    };

    UPDATE_KEYS();
  }
  return 0;
}
