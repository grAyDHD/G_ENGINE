#include "../includes/characterAnimator.h"

#define chocoboX chocobo.coordinate.x
#define chocoboY chocobo.coordinate.y

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

  initializeCharacterSprites(&chocobo, chocoboSprites);
  int chocoboStepCounter = 0;

  while (1) {
    moveCharacter(&chocobo);
    renderCharacter(&chocobo, chocoboStepCounter % 4, BedroomBitmap);
    chocoboStepCounter++;

    simpleWait(100);

    if (chocoboStepCounter >= 4) {
      chocoboStepCounter = 0;
      if (chocobo.direction == RIGHT) {
        setCharacterDirection(&chocobo, LEFT);
      } else {
        setCharacterDirection(&chocobo, RIGHT);
      }
    }
  }

  return 0;
}
