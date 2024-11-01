#ifndef SNAKE_H
#define SNAKE_H
#include "engine.h"

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

#endif
