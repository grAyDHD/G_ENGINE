#include "../includes/characterAnimator.h"
#include "core/timer.h"
#include "graphics/video.h"
#include "input/in.h"

#define chocoboX chocobo.coordinate.x
#define chocoboY chocobo.coordinate.y
#define playerX player.coordinate.x
#define playerY player.coordinate.y

typedef enum { false = 0, true = !false } bool;

void initializeCharacterSprites(
    Character *character, const void *spriteSheets[NUM_STATES][NUM_DIRECTIONS]);

void updateCurrentSpriteSheet(Character *character);

void setCharacterState(Character *character, STATE state);

void setCharacterDirection(Character *character, DIRECTION direction);

void setCharacterStateAndDirection(Character *character, STATE state,
                                   DIRECTION direction);

void moveCharacter(Character *character);

int main() {
  DSPC = MODE3 | BG2;

  m3_Background(BedroomBitmap);

  Character chocobo = {
      .direction = RIGHT,
      .state = WALK,
      .coordinate = {80, 75},
  };

  Character player = {
      .direction = DOWN,
      .state = IDLE,
      .coordinate = {120, 120},
  };

  initializeCharacterSprites(&chocobo, chocoboSprites);
  initializeCharacterSprites(&player, silverGirlSprites);

  int chocoboStepCounter = 0;
  int playerStepCounter = 0;

  while (1) {
    updateKeys();
    VBLANK();

    // Start by erasing sprite
    clearSpriteFrame(playerX, playerY, 32, BedroomBitmap);

    // Handle player input
    if (keyDown(U)) {
      setCharacterStateAndDirection(&player, WALK, UP);

      // Prevent player from moving into the collision box from the bottom
      if (!(playerX + 32 > 80 && playerX < 128 && playerY - 4 < 107 &&
            playerY >= 107)) {
        moveCharacter(&player);
      }
    } else if (keyDown(D)) {
      setCharacterStateAndDirection(&player, WALK, DOWN);

      // Prevent player from moving into the collision box from the top
      if (!(playerX + 32 > 80 && playerX < 128 && playerY + 4 + 32 > 75 &&
            playerY + 32 <= 75)) {
        moveCharacter(&player);
      }
    } else if (keyDown(L)) {
      setCharacterStateAndDirection(&player, WALK, LEFT);

      // Prevent player from moving into the collision box from the right
      if (!(playerX - 4 < 128 && playerX >= 128 && playerY + 32 > 75 &&
            playerY < 107)) {
        moveCharacter(&player);
      }
    } else if (keyDown(R)) {
      setCharacterStateAndDirection(&player, WALK, RIGHT);

      // Prevent player from moving into the collision box from the left
      if (!(playerX + 4 + 32 > 80 && playerX + 32 <= 80 && playerY + 32 > 75 &&
            playerY < 107)) {
        moveCharacter(&player);
      }
    }

    SpriteFrame32Bit(playerX, playerY, playerStepCounter % 4,
                     player.currentSpriteSheet, 4);
    restoreFrameBackground(playerX, playerY, 32, BedroomBitmap);

    if (keyReleased(U) | keyReleased(D) | keyReleased(L) | keyReleased(R)) {
      setCharacterState(&player, IDLE);
      playerStepCounter = 0;
    } else if (player.state != IDLE) {
      playerStepCounter++;
    }

    // Chocobo logic

    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    if (chocobo.state == WALK) {
      moveCharacter(&chocobo);
    }

    SpriteFrame32Bit(chocoboX, chocoboY, chocoboStepCounter,
                     chocobo.currentSpriteSheet, 4);

    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    chocoboStepCounter++;

    // handle chocobo direction change
    if (chocoboStepCounter >= 4) {
      chocoboStepCounter = 0;
      if (chocobo.direction == RIGHT) {
        setCharacterDirection(&chocobo, LEFT);
      } else {
        setCharacterDirection(&chocobo, RIGHT);
      }
    }

    simpleWait(30);
  }

  return 0;
}

//  chocobo starts facing right, x = 80 y = 75
//  4 steps 4 pixels each, 16 + 32 = 48 pixel wide box, 32 tall
//  player can not enter box (80,75), (80, 107), (128,75), (128,107)

void initializeCharacterSprites(
    Character *character,
    const void *spriteSheets[NUM_STATES][NUM_DIRECTIONS]) {
  for (int state = 0; state < NUM_STATES; state++) {
    for (int direction = 0; direction < NUM_DIRECTIONS; direction++) {
      character->sprites.spriteSheets[state][direction] =
          spriteSheets[state][direction];
    }
  }

  character->currentSpriteSheet =
      character->sprites.spriteSheets[character->state][character->direction];
};

void updateCurrentSpriteSheet(Character *character) {
  character->currentSpriteSheet =
      character->sprites.spriteSheets[character->state][character->direction];
}

void setCharacterState(Character *character, STATE state) {
  character->state = state;
  updateCurrentSpriteSheet(character);
}

void setCharacterDirection(Character *character, DIRECTION direction) {
  character->direction = direction;
  updateCurrentSpriteSheet(character);
}

void setCharacterStateAndDirection(Character *character, STATE state,
                                   DIRECTION direction) {
  character->state = state;
  character->direction = direction;
  updateCurrentSpriteSheet(character);
}

void moveCharacter(Character *character) {
  switch (character->direction) {
  case RIGHT:
    character->coordinate.x += 4;
    break;
  case LEFT:
    character->coordinate.x -= 4;
    break;
  case UP:
    character->coordinate.y -= 4;
    break;
  case DOWN:
    character->coordinate.y += 4;
    break;
  }
}
