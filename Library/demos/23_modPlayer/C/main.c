#include "core/typedefs.h"
#include "input/in.h"

int main() {
  s32 i;

  // initialize interrupts
  //  for (i = 0; i < IRQTABLE_NUM; i++) {
  //    IntrTable[i] = (u32)IrqNull;
  //  }
  // set VBlank interrupt
  //  IntrTable[IRQTABLE_VBL] = (u32)SndVSync;
  //  REG_DISPSTAT = DSTAT_VBL_IRQ;
  //  REG_IE = IRQ_VBL;

  //  SndInit(SND_FREQ_18157);

  // sound will actually start playing here (although the buffer
  // is empty at this point, so you won't hear anything)
  //  REG_IME = 1;

  //  i = 0;

  while (1) {
    //    VSync();

    // print the instructions
    //    FontPrint(0, 0, BGS(31), "Sound example");
    //    FontPrint(0, 1, BGS(31), "Start: play song");

    if (KEY_DOWN(ST)) {
      SndPlayMOD(MOD_AdventureBoy);
    }

    // Fill the next sound buffer, and update the music.
    // This can be done anywhere you like.
    // It takes a while if you have many channels running,
    // so it's best to do it after you're done with everything else.
    // If you think you could get missed frames, you might want to
    // do this in your VBlank function, just so you know for sure
    // it will happen every frame.

    // Wait until VBlank is over, to use BG color for profiling
    //    while (REG_VCOUNT != 0)
    //      ;
    //   SndUpdate();
  }

  return 0;
}
