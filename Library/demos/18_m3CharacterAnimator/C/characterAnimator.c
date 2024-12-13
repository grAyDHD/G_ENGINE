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
    SpriteFrame32Bit(chocoboX, chocoboY, 0, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(500);

    // Turnaround
    VBLANK();
    SpriteFrame32Bit(chocoboX, chocoboY, 0, chocobo.currentSpriteSheet, 4);
    restoreFrameBackground(chocoboX, chocoboY, 32, BedroomBitmap);
    simpleWait(100);

    // turn around draw frame 0
  }

  return 0;
}
