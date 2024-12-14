#include "../includes/characterAnimator.h"
#include "in.h"
#include "timer.h"

#define chocoboX chocobo.coordinate.x
#define chocoboY chocobo.coordinate.y

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

void renderCharacter(Character *character, int frame, const void *background) {
  VBLANK();
  clearSpriteFrame(character->coordinate.x, character->coordinate.y, 32,
                   background);
  SpriteFrame32Bit(character->coordinate.x, character->coordinate.y, frame,
                   character->currentSpriteSheet, 4);
  restoreFrameBackground(character->coordinate.x, character->coordinate.y, 32,
                         background);
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

    /*
    if (keyDown(U)) {
      setCharacterStateAndDirection(&player, WALK, UP);
    } else if (keyDown(D)) {
      setCharacterStateAndDirection(&player, WALK, UP);
    } else if (keyDown(L)) {
      setCharacterStateAndDirection(&player, WALK, UP);
    } else if (keyDown(R)) {
      setCharacterStateAndDirection(&player, WALK, UP);
    }

    if (keyReleased(U) | keyReleased(D) | keyReleased(L) | keyReleased(R)) {
      setCharacterState(&player, IDLE);
    }
    */

    // Render player
    renderCharacter(&player, 0, &BedroomBitmap);
    simpleWait(100);
    renderCharacter(&player, 1, &BedroomBitmap);
    simpleWait(100);
    renderCharacter(&player, 2, &BedroomBitmap);
    simpleWait(100);
    renderCharacter(&player, 3, &BedroomBitmap);
    simpleWait(100);

    /*
    // Chocobo logic
    if (chocobo.state == WALK) {
      moveCharacter(&chocobo);
    }
    renderCharacter(&chocobo, chocoboStepCounter % 4, &BedroomBitmap);
    chocoboStepCounter++;

    simpleWait(100);

    // handle chocobo direction change
    if (chocoboStepCounter >= 4) {
      chocoboStepCounter = 0;
      if (chocobo.direction == RIGHT) {
        setCharacterDirection(&chocobo, LEFT);
      } else {
        setCharacterDirection(&chocobo, RIGHT);
      }
    }
*/
  }
  return 0;
}
