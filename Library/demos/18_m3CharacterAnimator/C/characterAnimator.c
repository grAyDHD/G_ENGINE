#include "../includes/characterAnimator.h"

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
  }

  return 0;
}
