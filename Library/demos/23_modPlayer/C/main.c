#include "../data/dataSndData.h"
#include "audio/mixer.h"
#include "audio/modPlayer.h"
#include "graphics/draw.h"
#include "graphics/m3Text.h"
#include "graphics/video.h"

int main() {
  DSPC = MODE3 | BG2;

  initCharLookup();
  initializeFIFOInterrupts();
  modInit();
  gprintf(5, 5, "mixFreq: %d", modTiming.mixFreq);
  gprintf(5, 15, "rcpMixFreq: %d", modTiming.rcpMixFreq);
  printString(25, 25, "this prints a string");

  fillScreen(RGB(20, 1, 8));

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
  while (1) {

    // 1. Channel 0 data pointer
    if (modMixerChannel[0].data !=
        (s8 *)lastData) { // clearTextArea(65, 35, 80, 10);
      gprintf(65, 35, "%x", (u32)modMixerChannel[0].data);
      lastData = (u32)modMixerChannel[0].data;
    }

    // UPDATE WARNING
    // 2. Channel 0 position
    if (modMixerChannel[0].pos != lastPos) {
      // clearTextArea(60, 45, 80, 10);
      // gprintf(60, 45, "%x", modMixerChannel[0].pos);
      lastPos = modMixerChannel[0].pos;
    }

    // 3. Channel 0 increment
    if (modMixerChannel[0].inc != lastInc) {
      // clearTextArea(60, 55, 80, 10);
      gprintf(60, 55, "%x", modMixerChannel[0].inc);
      lastInc = modMixerChannel[0].inc;
    }

    // 4. Channel 0 volume
    if (modMixerChannel[0].vol != lastVol) {
      // clearTextArea(60, 65, 80, 10);
      gprintf(60, 65, "%x", modMixerChannel[0].vol);
      lastVol = modMixerChannel[0].vol;
    }

    // 5. Channel 0 length
    if (modMixerChannel[0].length != lastLength) {
      // clearTextArea(60, 75, 80, 10);
      gprintf(60, 75, "%x", modMixerChannel[0].length);
      lastLength = modMixerChannel[0].length;
    }

    // 6. Channel 0 loop length
    if (modMixerChannel[0].loopLength != lastLoopLength) {
      // clearTextArea(70, 85, 80, 10);
      gprintf(70, 85, "%x", modMixerChannel[0].loopLength);
      lastLoopLength = modMixerChannel[0].loopLength;
    }

    // 7. Current order
    if (modPlayer.curOrder != lastCurOrder) {
      // clearTextArea(105, 105, 80, 10);
      gprintf(105, 105, "%x", modPlayer.curOrder);
      lastCurOrder = modPlayer.curOrder;
    }

    // UPDATE WARNING
    // 8. Current row
    if (modPlayer.curRow != lastCurRow) {
      // clearTextArea(90, 115, 80, 10);
      //  gprintf(90, 115, "%x", modPlayer.curRow);
      lastCurRow = modPlayer.curRow;
    }

    // UPDATE WARNING
    // 9. Current tick
    if (modPlayer.tick != lastTick) {
      // clearTextArea(70, 125, 80, 10);
      //      gprintf(70, 125, "%x", modPlayer.tick);
      lastTick = modPlayer.tick;
    }

    // 10. MOD state
    if (modPlayer.state != lastState) {
      // clearTextArea(80, 135, 80, 10);
      gprintf(80, 135, "%x", modPlayer.state);
      lastState = modPlayer.state;
    }

    // 11. Samples until tick
    if (modTiming.samplesUntilMODTick != lastSamplesToTick) {
      //      clearTextArea(90, 145, 80, 10);
      //  gprintf(90, 145, "%x", modTiming.samplesUntilMODTick);
      lastSamplesToTick = modTiming.samplesUntilMODTick;
    }

    updateKeys();

    if (reload == 1) {
      modUpdate();
      reload = 0;
    }

    if (keyTapped(B)) {
      playMod(MOD_);
    }
  }

  return 0;
}

/*
    // Print out some info on the screen
    FontPrint(0, 4, BGS(31), "C0.data:");
    FontPrint(8, 4, BGS(31), htoa((s32)sndChannel[0].data));
    FontPrint(0, 5, BGS(31), "C0.pos:");
    FontPrint(8, 5, BGS(31), htoa((s32)sndChannel[0].pos));
    FontPrint(0, 6, BGS(31), "C0.inc:");
    FontPrint(8, 6, BGS(31), htoa((s32)sndChannel[0].inc));
    FontPrint(0, 7, BGS(31), "C0.vol:");
    FontPrint(8, 7, BGS(31), htoa((s32)sndChannel[0].vol));
    FontPrint(0, 8, BGS(31), "C0.len:");
    FontPrint(8, 8, BGS(31), htoa((s32)sndChannel[0].length));
    FontPrint(0, 9, BGS(31), "C0.loop:");
    FontPrint(8, 9, BGS(31), htoa((s32)sndChannel[0].loopLength));

    FontPrint(0, 11, BGS(31), "mod.curOrder:");
    FontPrint(15, 11, BGS(31), htoa(sndMod.curOrder));
    FontPrint(0, 12, BGS(31), "mod.curRow:");
    FontPrint(15, 12, BGS(31), htoa(sndMod.curRow));
    FontPrint(0, 13, BGS(31), "mod.tick:");
    FontPrint(15, 13, BGS(31), htoa(sndMod.tick));
    FontPrint(0, 14, BGS(31), "mod.state:");
    FontPrint(15, 14, BGS(31), htoa(sndMod.state));
    FontPrint(0, 15, BGS(31), "smpsToTick:");
    FontPrint(15, 15, BGS(31), htoa(sndVars.samplesUntilMODTick));
*/
