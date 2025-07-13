#include "../includes/directSound.h"

int main(void) {
  DSPC = MODE3 | BG2;

  initMixChannels();
  setMixChannels();
  irqMaster(ON);  // now enable interrupts
  initMonoFIFO(); // is now reading from buffer

  while (1) {
    if (reload == 1) {
      mixAudio();
      reload = 0;
    }

    updateKeys();

    if (keyTapped(ST)) {
      if (channel[0].isPlaying) {
        stopMusicLoop();
      } else {
        startMusicLoop();
      }
    }

    if (keyTapped(B)) {
      triggerKick();
    }

    if (keyTapped(A)) {
      triggerSnare();
    }

    if (keyTapped(RT)) {
      triggerHat();
    }

    if (keyTapped(LT)) {
      triggerHat();
    }
  }
  return 0;
}

void setMixChannels() {
  // Channel 0: Music loop (tharp)
  channel[0].data = tharp16k;
  channel[0].length = tharp16klen;
  channel[0].volume = 32;
  channel[0].looping = 1;

  // Channel 1: Sample A (one-shot kick)
  channel[1].data = kick16k;
  channel[1].length = kick16klen;
  channel[1].volume = 32;

  // Channel 2: Sample B (one-shot snare)
  channel[2].data = snare16k;
  channel[2].length = snare16klen;
  channel[2].volume = 32;

  // Channel 3: Sample B (one-shot hi hat)
  channel[3].data = hat16k;
  channel[3].length = hat16klen;
  channel[3].volume = 32;
}

void startMusicLoop() {
  channel[0].position = 0;
  channel[0].isPlaying = 1;
}

void stopMusicLoop() { channel[0].isPlaying = 0; }

void triggerKick() {
  channel[1].position = 0;
  channel[1].isPlaying = 1;
}

void triggerSnare() {
  channel[2].position = 0;
  channel[2].isPlaying = 1;
}

void triggerHat() {
  channel[3].position = 0;
  channel[3].isPlaying = 1;
}
