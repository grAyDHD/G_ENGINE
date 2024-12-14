#include "../includes/characterAnimator.h"
#include "gfx.h"
#include "in.h"
#include "timer.h"

#define chocoboX chocobo.coordinate.x
#define chocoboY chocobo.coordinate.y
#define playerX player.coordinate.x
#define playerY player.coordinate.y

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

    clearSpriteFrame(playerX, playerY, 32, BedroomBitmap);
    if (keyDown(U)) {
      setCharacterStateAndDirection(&player, WALK, UP);
      player.coordinate.y -= 4;
    } else if (keyDown(D)) {
      setCharacterStateAndDirection(&player, WALK, DOWN);
      player.coordinate.y += 4;
    } else if (keyDown(L)) {
      setCharacterStateAndDirection(&player, WALK, LEFT);
      player.coordinate.x -= 4;
    } else if (keyDown(R)) {
      setCharacterStateAndDirection(&player, WALK, RIGHT);
      player.coordinate.x += 4;
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

    SpriteFrame32Bit(chocoboX, chocoboY, chocoboStepCounter % 4,
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

    simpleWait(50);
  }

  return 0;
}
