#include "engine.h"

#define COLOR(r, g, b) (((r) & 0x1F) << 10 | ((g) & 0x1F) << 5 | ((b) & 0x1F))
#define GW 30 // GRID WIDTH = SW/8
#define GH 20 // GRID HEIGHT = SH/8

u16 keyCache = 0;
u16 prevKeyCache = 0;

enum Direction { UP, DOWN, LEFT, RIGHT };

typedef struct {
  Coordinate xy;
  struct Segment *next;
} Segment;

typedef struct {
  Segment *head;
  Segment *tail;
  int length;
  enum Direction direction;
} Snake;
void clearSnake(Snake *snake) {
  int black = COLOR(0, 0, 0);
  drawRect(snake->head->xy, 8, 8, black);
};

void handleInput(Snake *snake) {
  switch (snake->direction) {
  case (UP):
    if (INPUT(L)) {
      snake->direction = LEFT;
    } else if (INPUT(R)) {
      snake->direction = RIGHT;
    }
    break;
  case (DOWN):
    if (INPUT(L)) {
      snake->direction = LEFT;
    } else if (INPUT(R)) {
      snake->direction = RIGHT;
    }
    break;
  case (LEFT):
    if (INPUT(U)) {
      snake->direction = UP;
    } else if (INPUT(D)) {
      snake->direction = DOWN;
    }
    break;
  case (RIGHT):
    if (INPUT(U)) {
      snake->direction = UP;
    } else if (INPUT(D)) {
      snake->direction = DOWN;
    }
    break;
  }
};

void updateSnakePosition(Snake *snake) {
  switch (snake->direction) {
  case UP:
    snake->head->xy.y -= 8; // Move up by one grid unit (8 pixels)
    break;
  case DOWN:
    snake->head->xy.y += 8; // Move down by one grid unit (8 pixels)
    break;
  case LEFT:
    snake->head->xy.x -= 8; // Move left by one grid unit (8 pixels)
    break;
  case RIGHT:
    snake->head->xy.x += 8; // Move right by one grid unit (8 pixels)
    break;
  }
};

void drawSnake(Snake *snake) {
  int green = COLOR(0, 20, 7);
  drawRect(snake->head->xy, 8, 8, green);
};

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
