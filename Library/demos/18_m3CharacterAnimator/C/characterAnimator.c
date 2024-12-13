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

int main() {
  DSPC = MODE3 | BG2;

  m3_Background(BedroomBitmap);

  Character chocobo = {
      .direction = RIGHT,
      .state = WALK,
      .coordinate = {80, 75},
  };

  initializeCharacterSprites(&chocobo, chocoboSprites);

  while (1) {
    setCharacterDirection(&chocobo, RIGHT);

    // draw chocobo frame 0
    VBLANK();
    SpriteFrame32Bit(chocoboX, chocoboY, 0, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);

    // step right draw frame 1
    VBLANK();
    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    chocobo.coordinate.x += 4;
    SpriteFrame32Bit(chocoboX, chocoboY, 1, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);

    // step right draw frame 2
    VBLANK();
    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    chocobo.coordinate.x += 4;
    SpriteFrame32Bit(chocoboX, chocoboY, 2, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);

    // step draw frame 3
    VBLANK();
    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    chocobo.coordinate.x += 4;
    SpriteFrame32Bit(chocoboX, chocoboY, 3, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);

    // draw chocobo frame 0, last step first frame, before turnaround
    VBLANK();
    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    chocobo.coordinate.x += 4;
    SpriteFrame32Bit(chocoboX, chocoboY, 0, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(500);

    // Turnaround
    setCharacterDirection(&chocobo, LEFT);

    VBLANK();
    SpriteFrame32Bit(chocoboX, chocoboY, 0, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);

    // step left draw frame 1
    VBLANK();
    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    chocobo.coordinate.x -= 4;
    SpriteFrame32Bit(chocoboX, chocoboY, 1, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);

    // step left draw frame 2
    VBLANK();
    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    chocobo.coordinate.x -= 4;
    SpriteFrame32Bit(chocoboX, chocoboY, 2, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);

    // step draw frame 3
    VBLANK();
    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    chocobo.coordinate.x -= 4;
    SpriteFrame32Bit(chocoboX, chocoboY, 3, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);

    // draw chocobo frame 0, last step first frame, before turnaround
    VBLANK();
    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    chocobo.coordinate.x -= 4;
    SpriteFrame32Bit(chocoboX, chocoboY, 0, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(500);

    // Turnaround
  }

  return 0;
}
