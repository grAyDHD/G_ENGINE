#include "../includes/23_modPlayer.h"

MOD sndMod;
SOUND_VARS sndVars;

const u16 noteFreqTable[] = {
    // Finetune 0
    2090, 2215, 2348, 2485, 2639, 2796, 2963, 3139, 3326, 3523, 3728,
    3950, // C0-B0
    4181, 4430, 4697, 4971, 5279, 5593, 5926, 6279, 6653, 7046, 7457,
    7901, // C1-B1
    8363, 8860, 9395, 9943, 10559, 11186, 11852, 12559, 13306, 14092, 14914,
    15838, // C2-B2
    16726, 17720, 18839, 19886, 21180, 22372, 23705, 25208, 26713, 28185, 29829,
    31677, // C3-B3
    33453, 35441, 37679, 39772, 42613, 44744, 47727, 50416, 53426, 56818, 59659,
    63920, // C4-B4
           // Finetune 1
    2105, 2231, 2364, 2503, 2655, 2809, 2977, 3156, 3345, 3544, 3752,
    3977, // C0-B0
    4211, 4463, 4728, 5006, 5310, 5619, 5955, 6313, 6690, 7088, 7504,
    7954, // C1-B1
    8422, 8926, 9469, 10026, 10621, 11256, 11931, 12648, 13406, 14204, 15040,
    15909, // C2-B2
    16884, 17897, 18939, 20109, 21306, 22512, 23863, 25386, 26913, 28409, 30080,
    31960, // C3-B3
    33769, 35795, 38080, 40219, 42613, 45310, 47727, 51136, 54235, 56818, 60670,
    63920, // C4-B4
           // Finetune 2
    2120, 2248, 2380, 2524, 2671, 2831, 2997, 3178, 3364, 3565, 3775,
    4003, // C0-B0
    4241, 4496, 4760, 5048, 5342, 5663, 5995, 6357, 6728, 7130, 7551,
    8007, // C1-B1
    8482, 8993, 9520, 10111, 10685, 11327, 12011, 12738, 13456, 14261, 15103,
    16051, // C2-B2
    16964, 17987, 19040, 20223, 21434, 22655, 24023, 25568, 26913, 28636, 30335,
    32248, // C3-B3
    34090, 36157, 38080, 40676, 43127, 45310, 48372, 51136, 54235, 57734, 60670,
    65082, // C4-B4
           // Finetune 3
    2135, 2262, 2399, 2542, 2691, 2849, 3023, 3201, 3389, 3593, 3808,
    4031, // C0-B0
    4271, 4525, 4798, 5084, 5382, 5699, 6046, 6403, 6779, 7187, 7616,
    8062, // C1-B1
    8543, 9062, 9596, 10169, 10781, 11399, 12093, 12829, 13558, 14375, 15232,
    16124, // C2-B2
    17127, 18170, 19244, 20338, 21563, 22799, 24186, 25752, 27117, 28867, 30594,
    32248, // C3-B3
    34418, 36525, 38489, 40676, 43127, 45891, 48372, 51877, 54235, 57734, 61716,
    65082, // C4-B4
           // Finetune 4
    2151, 2279, 2415, 2560, 2711, 2872, 3043, 3224, 3415, 3615, 3832,
    4058, // C0-B0
    4302, 4559, 4830, 5120, 5423, 5745, 6087, 6449, 6831, 7231, 7664,
    8116, // C1-B1
    8604, 9131, 9674, 10256, 10847, 11509, 12175, 12922, 13662, 14492, 15362,
    16270, // C2-B2
    17209, 18262, 19348, 20572, 21694, 23093, 24350, 25938, 27324, 29101, 30858,
    32541, // C3-B3
    34418, 36525, 38908, 41144, 43652, 46487, 49034, 51877, 55069, 58681, 61716,
    65082, // C4-B4
           // Finetune 5
    2166, 2297, 2431, 2578, 2732, 2891, 3064, 3248, 3441, 3645, 3865,
    4095, // C0-B0
    4333, 4595, 4863, 5157, 5464, 5782, 6129, 6496, 6883, 7290, 7731,
    8191, // C1-B1
    8667, 9201, 9727, 10315, 10946, 11584, 12258, 13016, 13767, 14610, 15495,
    16419, // C2-B2
    17376, 18451, 19454, 20691, 21960, 23243, 24517, 26128, 27534, 29340, 31126,
    32839, // C3-B3
    34752, 36902, 38908, 41622, 44191, 46487, 49034, 52640, 55069, 58681, 62799,
    65535, // C4-B4
           // Finetune 6
    2182, 2312, 2451, 2597, 2749, 2914, 3085, 3271, 3468, 3675, 3890,
    4123, // C0-B0
    4365, 4624, 4903, 5195, 5498, 5829, 6171, 6543, 6937, 7350, 7781,
    8247, // C1-B1
    8730, 9249, 9806, 10405, 11013, 11659, 12343, 13111, 13874, 14730, 15563,
    16495, // C2-B2
    17461, 18546, 19667, 20811, 22095, 23395, 24686, 26320, 27748, 29583, 31126,
    33143, // C3-B3
    35093, 37286, 39335, 41622, 44191, 47099, 49715, 52640, 55930, 59659, 62799,
    65535, // C4-B4
           // Finetune 7
    2198, 2330, 2468, 2616, 2770, 2934, 3112, 3296, 3488, 3697, 3916,
    4152, // C0-B0
    4397, 4660, 4937, 5233, 5541, 5868, 6225, 6592, 6977, 7395, 7832,
    8305, // C1-B1
    8794, 9321, 9888, 10466, 11082, 11736, 12472, 13208, 13982, 14791, 15699,
    16649, // C2-B2
    17633, 18643, 19776, 20933, 22233, 23549, 25031, 26515, 27965, 29583, 31399,
    33453, // C3-B3
    35441, 37286, 39772, 42112, 44744, 47099, 50416, 53426, 55930, 59659, 62799,
    65535, // C4-B4
           // Finetune 8
    1973, 2090, 2215, 2348, 2485, 2639, 2796, 2963, 3139, 3326, 3523,
    3728, // C0-B0
    3946, 4181, 4430, 4697, 4971, 5279, 5593, 5926, 6279, 6653, 7046,
    7457, // C1-B1
    7901, 8363, 8860, 9395, 9943, 10559, 11186, 11852, 12559, 13306, 14092,
    14914, // C2-B2
    15838, 16726, 17720, 18839, 19886, 21180, 22372, 23705, 25208, 26713, 28185,
    29829, // C3-B3
    31677, 33453, 35441, 37679, 39772, 42613, 44744, 47727, 50416, 53426, 56818,
    59659, // C4-B4
           // Finetune 9
    1988, 2105, 2231, 2364, 2503, 2651, 2814, 2977, 3156, 3345, 3544,
    3752, // C0-B0
    3977, 4211, 4463, 4728, 5006, 5303, 5628, 5955, 6313, 6690, 7088,
    7504, // C1-B1
    7954, 8422, 8926, 9469, 10026, 10621, 11256, 11931, 12648, 13406, 14204,
    15040, // C2-B2
    15909, 16884, 17897, 18939, 20109, 21306, 22512, 23863, 25386, 26913, 28409,
    30080, // C3-B3
    31960, 33769, 35795, 38080, 40219, 42613, 45310, 47727, 51136, 54235, 56818,
    60670, // C4-B4
           // Finetune 10
    2001, 2120, 2248, 2380, 2524, 2671, 2831, 2997, 3178, 3364, 3565,
    3775, // C0-B0
    4003, 4241, 4496, 4760, 5048, 5342, 5663, 5995, 6357, 6728, 7130,
    7551, // C1-B1
    8007, 8482, 8993, 9520, 10111, 10685, 11327, 12011, 12738, 13456, 14261,
    15103, // C2-B2
    16051, 16964, 17987, 19040, 20223, 21434, 22655, 24023, 25568, 26913, 28636,
    30335, // C3-B3
    32248, 34090, 36157, 38080, 40676, 43127, 45310, 48372, 51136, 54235, 57734,
    60670, // C4-B4
           // Finetune 11
    2017, 2135, 2262, 2399, 2542, 2691, 2849, 3023, 3201, 3389, 3593,
    3808, // C0-B0
    4035, 4271, 4525, 4798, 5084, 5382, 5699, 6046, 6403, 6779, 7187,
    7616, // C1-B1
    8080, 8543, 9062, 9596, 10169, 10781, 11399, 12093, 12829, 13558, 14375,
    15232, // C2-B2
    16197, 17127, 18170, 19244, 20338, 21563, 22799, 24186, 25752, 27117, 28867,
    30594, // C3-B3
    32541, 34418, 36525, 38489, 40676, 43127, 45891, 48372, 51877, 54235, 57734,
    61716, // C4-B4
           // Finetune 12
    2031, 2151, 2279, 2415, 2560, 2711, 2872, 3043, 3224, 3415, 3623,
    3832, // C0-B0
    4063, 4302, 4559, 4830, 5120, 5423, 5745, 6087, 6449, 6831, 7246,
    7664, // C1-B1
    8135, 8604, 9131, 9674, 10256, 10847, 11509, 12175, 12922, 13662, 14492,
    15362, // C2-B2
    16270, 17209, 18262, 19348, 20572, 21694, 23093, 24350, 25938, 27324, 29101,
    30858, // C3-B3
    32541, 34418, 36525, 38908, 41144, 43652, 46487, 49034, 51877, 55069, 58681,
    61716, // C4-B4
           // Finetune 13
    2045, 2166, 2297, 2431, 2578, 2732, 2891, 3064, 3248, 3441, 3645,
    3865, // C0-B0
    4090, 4333, 4595, 4863, 5157, 5464, 5782, 6129, 6496, 6883, 7290,
    7731, // C1-B1
    8191, 8667, 9201, 9727, 10315, 10946, 11584, 12258, 13016, 13767, 14610,
    15495, // C2-B2
    16419, 17376, 18451, 19454, 20691, 21960, 23243, 24517, 26128, 27534, 29340,
    31126, // C3-B3
    32839, 34752, 36902, 38908, 41622, 44191, 46487, 49034, 52640, 55069, 58681,
    62799, // C4-B4
           // Finetune 14
    2061, 2182, 2312, 2451, 2597, 2749, 2914, 3085, 3271, 3468, 3675,
    3890, // C0-B0
    4123, 4365, 4624, 4903, 5195, 5498, 5829, 6171, 6543, 6937, 7350,
    7781, // C1-B1
    8247, 8730, 9249, 9806, 10405, 11013, 11659, 12343, 13111, 13874, 14730,
    15563, // C2-B2
    16495, 17461, 18546, 19667, 20811, 22095, 23395, 24686, 26320, 27748, 29583,
    31126, // C3-B3
    33143, 35093, 37286, 39335, 41622, 44191, 47099, 49715, 52640, 55930, 59659,
    62799, // C4-B4
           // Finetune 15
    2076, 2198, 2330, 2468, 2616, 2770, 2934, 3112, 3296, 3488, 3697,
    3916, // C0-B0
    4152, 4397, 4660, 4937, 5233, 5541, 5868, 6225, 6592, 6977, 7395,
    7832, // C1-B1
    8305, 8794, 9321, 9888, 10466, 11082, 11736, 12472, 13208, 13982, 14791,
    15699, // C2-B2
    16649, 17633, 18643, 19776, 20933, 22233, 23549, 25031, 26515, 27965, 29583,
    31399, // C3-B3
    33453, 35441, 37286, 39772, 42112, 44744, 47099, 50416, 53426, 55930, 59659,
    62799, // C4-B4

}; // noteFreqTable

static void MODPlayNote(u32 channelIdx, u32 note, u32 sampleIdx, u32 effect,
                        u32 param) {
  AudioChannel *sndChn;
  MOD_CHANNEL *modChn;
  const SAMPLE_HEADER *sample;
  u8 finetune;

  // Here's that special case that they didn't specify a sample before playing a
  // note
  if (sampleIdx == MOD_NO_SAMPLE) {
    return;
  }

  // These make things less cumbersome
  sndChn = &channel[channelIdx];
  modChn = &sndMod.channel[channelIdx];
  sample = &sndMod.sample[sampleIdx];

  // 60 notes total, and one full set of notes for each finetune level
  modChn->frequency = noteFreqTable[sample->finetune * 60 + note];

  // Set up the mixer channel
  sndChn->data = sample->smpData;
  sndChn->position = 0;
  sndChn->increment =
      modChn->frequency * sndVars.rcpMixFreq >> 16; // Explained below

  // Next member is volume, but skip setting it because it doesn't change unless
  // a new sample was specified, in which case it was already set back in
  // MODProcessRow

  // If looping, use loopStart+loopLength as the ending point,
  // otherwise just use normal length.
  // Length and loop length are still half what the real size is
  // (to fit in 16 bits, as per MOD format), so they need to be
  // shifted left 1. They also need to be shifted left 12 because
  // the mixer compares the 12-bit fixed-point position against
  // them directly, so that comes to a total shift left of 13
  sndChn->length =
      (sample->loopLength != 0 ? sample->loopStart + sample->loopLength
                               : sample->length)
      << 13;
  sndChn->loopLength = sample->loopLength << 13;

} // MODPlayNote

static void MODProcessRow() {
  s32 curChannel;

  for (curChannel = 0; curChannel < 4; curChannel++) {
    u8 note, sample, effect, param;

    // Read in the pattern data, advancing rowPtr to the next channel in the
    // process
    note = *sndMod.rowPtr++;
    sample = *sndMod.rowPtr++;
    effect = *sndMod.rowPtr++;
    param = *sndMod.rowPtr++;

    // Use sample memory if no sample, or set sample memory if there is one
    if (sample == MOD_NO_SAMPLE) {
      sample = sndMod.channel[curChannel].sample;
    } else {
      // Set sample memory
      sndMod.channel[curChannel].sample = sample;
      // Another tricky thing to know about MOD: Volume is only set when
      // specifying new samples, NOT when playing notes, and it is set even
      // when there is a sample, but no note specified (although the sample
      // playing doesn't change in that case)
      sndMod.channel[curChannel].vol = sndMod.sample[sample].vol;
      channel[curChannel].volume = sndMod.channel[curChannel].vol;
    }

    // See if there's any note to play
    if (note != MOD_NO_NOTE) {
      MODPlayNote(curChannel, note, sample, effect, param);
    }
  }

} // MODProcessRow

void MODUpdate() {
  if (++sndMod.tick >= sndMod.speed) {
    sndMod.tick = 0;

    if (++sndMod.curRow >= 64) {
      sndMod.curRow = 0;

      if (++sndMod.curOrder >= sndMod.orderCount) {
        sndMod.state = MOD_STATE_STOP;
        return;
      } else {
        sndMod.rowPtr = sndMod.pattern[sndMod.order[sndMod.curOrder]];
      }
    }

    MODProcessRow();
  } else {
    // We'll update the non row-tick effects here later
  }

} // MODUpdate

static void MODSetTempo(u32 tempo) {
  u32 modFreq;

  sndMod.tempo = tempo;
  modFreq = (tempo * 2) / 5;

  sndVars.samplesUntilMODTick -= sndVars.samplesPerMODTick;
  sndVars.samplesPerMODTick = sndVars.mixFreq / modFreq;
  sndVars.samplesUntilMODTick += sndVars.samplesPerMODTick;

} // MODSetTempo

// This is only called by SndUpdate
void SndMix(u32 samplesToMix) {
  s32 i, curChn;

  // If you want to use a higher frequency than 18157,
  // you'll need to make this bigger.
  // To be safe, it would be best to set it to the buffer
  // size of the highest frequency we allow in freqTable
  s16 tempBuffer[BUFFER_SIZE];

  // zero as much of the buffer as we'll actually use,
  // rounding samples up to nearest 2 for memset32
  i = 0;
  Dma3(tempBuffer, &i, (samplesToMix + 1) * sizeof(s16) / 4, DMA_MEMSET32);

  for (curChn = 0; curChn < 4; curChn++) {
    AudioChannel *chnPtr = &channel[curChn];

    // check special active flag value
    if (chnPtr->data != 0) {
      // this channel is active, so mix its data into the intermediate buffer
      for (i = 0; i < samplesToMix; i++) {
        // mix a sample into the intermediate buffer
        tempBuffer[i] += chnPtr->data[chnPtr->position >> 12] * chnPtr->volume;
        chnPtr->position += chnPtr->increment;

        // loop the sound if it hits the end
        if (chnPtr->position >= chnPtr->length) {
          // check special loop on/off flag value
          if (chnPtr->loopLength == 0) {
            // disable the channel and break from the i loop
            chnPtr->data = 0;
            i = samplesToMix;
          } else {
            // loop back
            while (chnPtr->position >= chnPtr->length) {
              chnPtr->position -= chnPtr->loopLength;
            }
          }
        }
      } // end for i = 0 to bufSize
    } // end data != 0
  } // end channel loop

  // now downsample the 16-bit buffer and copy it into the actual playing buffer
  for (i = 0; i < samplesToMix; i++) {
    // >>6 to divide off the volume, >>2 to divide by 4 channels
    // to prevent overflow. Could make a define for this up with
    // SOUND_MAX_CHANNELS, but I'll hardcode it for now
    sndVars.curMixBuffer[i] = tempBuffer[i] >> 8;
  }

  // curMixBuffer will get reset on next VBlank anyway, so we can
  // move the pointer forward to avoid having to make a variable
  // to keep track of how many samples have been mixed so far
  sndVars.curMixBuffer += samplesToMix;

} // SndMix

void SndUpdate() {
  s32 samplesLeft = BUFFER_SIZE; // 256

  while (samplesLeft > 0) {
    if (sndVars.samplesUntilMODTick == 0 && sndMod.state == MOD_STATE_PLAY) {
      MODUpdate();
      sndVars.samplesUntilMODTick = sndVars.samplesPerMODTick;
    }

    if (sndVars.samplesUntilMODTick < samplesLeft &&
        sndMod.state == MOD_STATE_PLAY) {
      SndMix(sndVars.samplesUntilMODTick);
      samplesLeft -= sndVars.samplesUntilMODTick;
      sndVars.samplesUntilMODTick = 0;
    } else {
      SndMix(samplesLeft);
      sndVars.samplesUntilMODTick -= samplesLeft;
      samplesLeft = 0;
    }
  }
}

void SndPlayMOD(u32 modIdx) {
  const MOD_HEADER *modHeader = &dModTable[modIdx];

  sndMod.sample = modHeader->sample;
  sndMod.pattern = modHeader->pattern;
  sndMod.order = modHeader->order;
  sndMod.orderCount = modHeader->orderCount;
  sndMod.curOrder = 0;
  sndMod.curRow = 0;
  sndMod.tick = 0;
  sndMod.speed = MOD_DEFAULT_SPEED;
  sndMod.rowPtr = sndMod.pattern[sndMod.order[0]];
  sndMod.state = MOD_STATE_PLAY;

  // Set to default
  MODSetTempo(MOD_DEFAULT_TEMPO);
  sndVars.samplesUntilMODTick = 0;

} // SndPlayMOD

void initDebugDisplay() {
  // Draw static labels once at startup - never cleared
  printString(10, 10, "Pattern:");
  printString(10, 22, "Row:");
  //  printString(10, 34, "Tempo:");
  gprintf(10, 34, "sample data start address: %x", (u32)&channel[0].data);

  //  FontPrint(0, 0, BGS(31), "Sound example");
  //  FontPrint(0, 1, BGS(31), "Start: play song");
}

void updateDebugNumbers(int pattern, int row, int tempo) {
  // Only clear/redraw the number portions
  clearTextArea(70, 10, 30, 11); // Clear just the number area
  gprintf(70, 10, "%d", pattern);

  clearTextArea(70, 22, 30, 11);
  gprintf(70, 22, "%d", row);

  //  clearTextArea(70, 34, 30, 11);
  //  clearTextArea(70, 34, 30, 11);
  //  gprintf(70, 34, "%d", tempo);

  // Print out some info on the screen
  /*
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
}
