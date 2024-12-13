#include "../includes/characterAnimator.h"
#include "timer.h"

#define chocoboX chocobo.coordinate.x
#define chocoboY chocobo.coordinate.y

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
    VBLANK();
    // draw chocobo frame 0
    SpriteFrame32Bit(chocoboX, chocoboY, 0, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);

    VBLANK();
    // step right draw frame 1
    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    chocobo.coordinate.x += 4;
    SpriteFrame32Bit(chocoboX, chocoboY, 1, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);

    VBLANK();
    // step right draw frame 2

    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    chocobo.coordinate.x += 4;
    SpriteFrame32Bit(chocoboX, chocoboY, 2, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);
    VBLANK();
    // step draw frame 3

    clearSpriteFrame(chocoboX, chocoboY, 32, BedroomBitmap);
    chocobo.coordinate.x += 4;
    SpriteFrame32Bit(chocoboX, chocoboY, 3, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);
  }

  return 0;
}
