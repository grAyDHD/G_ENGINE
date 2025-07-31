#include "../data/dataSndData.h"
#include "audio/mixer.h"
#include "audio/modPlayer.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"

#define DSPS (*(volatile u16 *)0x04000004)

int main() {
  DSPC = MODE3 | BG2;

  initCharLookup();
  initializeFIFOInterrupts();
  modInit();
  gprintf(5, 5, "mixFreq: %d", modTiming.mixFreq);
  gprintf(5, 15, "rcpMixFreq: %d", modTiming.rcpMixFreq);

  // 1. Channel 0 data pointer
  printString(0, 35, "C0.data:");
  // 2. Channel 0 position
  printString(0, 45, "C0.pos:");
  // 3. Channel 0 increment
  printString(0, 55, "C0.inc:");
  // 4. Channel 0 volume
  printString(0, 65, "C0.vol:");
  // 5. Channel 0 length
  printString(0, 75, "C0.len:");
  // 6. Channel 0 loop length
  printString(0, 85, "C0.loop:");
  // 7. Current order
  printString(0, 105, "mod.curOrder:");
  // 8. Current row
  printString(0, 115, "mod.curRow:");
  // 9. Current tick
  printString(0, 125, "mod.tick:");
  // 10. MOD state
  printString(0, 135, "mod.state:");
  // 11. Samples until tick
  printString(0, 145, "smpsToTick:");

  static u32 lastData = 0;
  static u32 lastPos = 0;
  static u32 lastInc = 0;
  static u32 lastVol = 0;
  static u32 lastLength = 0;
  static u32 lastLoopLength = 0;
  static u8 lastCurOrder = 0;
  static u8 lastCurRow = 0;
  static u8 lastTick = 0;
  static u8 lastState = 0;
  static s16 lastSamplesToTick = 0;

  static int frameCount = 0;

  while (1) {
    updateKeys();

    if (reload == 1) {
      modUpdate();
      reload = 0;
    }

    if (keyTapped(B)) {
      playMod(MOD_BIT_Introtune);
    }
    /*
        // 1. Channel 0 data pointer
        if (modMixerChannel[0].data != (s8 *)lastData) {
          clearTextArea(127, 35, 55, 9);
          gprintf(105, 35, "%x", (u32)modMixerChannel[0].data);
          lastData = (u32)modMixerChannel[0].data;
        }
        // UPDATE WARNING
        // 2. Channel 0 position
        if (modMixerChannel[0].pos != lastPos && DSPS & 0x0001) {
          clearTextArea(127, 45, 55, 9);
          gprintf(105, 45, "%x", modMixerChannel[0].pos);
          lastPos = modMixerChannel[0].pos;
        }

        // 3. Channel 0 increment
        if (modMixerChannel[0].inc != lastInc) {
          clearTextArea(127, 55, 55, 9);
          gprintf(105, 55, "%x", modMixerChannel[0].inc);
          lastInc = modMixerChannel[0].inc;
        }

        // 4. Channel 0 volume
        if (modMixerChannel[0].vol != lastVol && frameCount % 5 == 1 &&
            DSPS & 0x0001) {
          clearTextArea(127, 65, 55, 9);
          gprintf(105, 65, "%x", modMixerChannel[0].vol);
          lastVol = modMixerChannel[0].vol;
        }

        // 5. Channel 0 length
        if (modMixerChannel[0].length != lastLength) {
          clearTextArea(127, 75, 55, 9);
          gprintf(105, 75, "%x", modMixerChannel[0].length);
          lastLength = modMixerChannel[0].length;
        }

        // 6. Channel 0 loop length
        if (modMixerChannel[0].loopLength != lastLoopLength) {
          clearTextArea(127, 85, 55, 9);
          gprintf(105, 85, "%x", modMixerChannel[0].loopLength);
          lastLoopLength = modMixerChannel[0].loopLength;
        }

        // 7. Current order
        if (modPlayer.curOrder != lastCurOrder) {
          clearTextArea(127, 105, 55, 9);
          gprintf(105, 105, "%x", modPlayer.curOrder);
          lastCurOrder = modPlayer.curOrder;
        }

        // UPDATE WARNING
        // 8. Current row
        if (modPlayer.curRow != lastCurRow && frameCount % 5 == 2 &&
            DSPS & 0x0001) {
          clearTextArea(127, 115, 55, 9);
          gprintf(105, 115, "%x", modPlayer.curRow);
          lastCurRow = modPlayer.curRow;
        }

    // UPDATE WARNING
    // 9. Current tick
    if (modPlayer.tick != lastTick) {
      clearTextArea(127, 125, 55, 9);
      gprintf(105, 125, "%x", modPlayer.tick);
      lastTick = modPlayer.tick;
    }
    // 10. MOD state
    if (modPlayer.state != lastState) {
      clearTextArea(127, 135, 55, 9);
      gprintf(105, 135, "%x", modPlayer.state);
      lastState = modPlayer.state;
    }
      */

    /*
    // UPDATE WARNING
    // 11. Samples until tick
    if (modTiming.samplesUntilMODTick != lastSamplesToTick) {
      clearTextArea(127, 145, 55, 9);
      gprintf(105, 145, "%x", modTiming.samplesUntilMODTick);
      lastSamplesToTick = modTiming.samplesUntilMODTick;
    }
*/
    frameCount++;
  }

  return 0;
}
