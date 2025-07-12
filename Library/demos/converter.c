#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef int32_t s32;
typedef u_int32_t u32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef int8_t s8;

// songFreq = tempo*2/5;

#define ASSERT(condition)                                                      \
  if (!(condition)) {                                                          \
    printf("ERROR: %s failed at line %d\n", #condition, __LINE__);             \
    exit(1);                                                                   \
  }

typedef struct __attribute__((packed)) {
  char name[22];
  u16 length;
  u8 finetune;
  u8 volume;
  u16 loopStart;  // swap bytes, word count (2 byte words)
  u16 loopLength; // swap bytes, word count (2 byte words)

} SAMPLE_HEADER;

typedef struct {
  char signature[5];
  char songName[20];

  SAMPLE_HEADER sample[31];

  u8 order[128];
  u8 orderCount;
  u8 **pattern;
  u8 patternCount;

  s8 *sampleData; // stored at end of file
} MOD_HEADER;

char name[20];
SAMPLE_HEADER sample[31];
u8 order[128];
u8 **pattern;
u8 orderCount;
u8 patternCount;

MOD_HEADER modHeader;

int main(int argc, char **argv) {
  s32 i;

  FILE *modFile;
  ASSERT(argc == 2);

  // rb is read binary
  modFile = fopen(argv[1], "rb");
  ASSERT(modFile);

  // verify signature, for now only M.K. or 4CHN
  fseek(modFile, 1080, SEEK_SET);
  fread(modHeader.signature, 1, 4, modFile);
  modHeader.signature[4] = '\0';
  ASSERT(strncmp(modHeader.signature, "M.K.", 4) == 0 ||
         strncmp(modHeader.signature, "4CHN", 4) == 0);
  printf("Signature: %s\n", modHeader.signature);

  // get song name, first 20 bytes of file
  fseek(modFile, 0, SEEK_SET);
  fread(modHeader.songName, 1, 20, modFile);
  modHeader.songName[19] = '\0';
  printf("Song name: %s\n", modHeader.songName);

  for (i = 0; i < 31; i++) {
    fread(&modHeader.sample[i], sizeof(SAMPLE_HEADER), 1, modFile);
    modHeader.sample[i].name[21] = '\0';
    printf("sample %d name: %s\n", i, modHeader.sample[i].name);

    modHeader.sample[i].length = (((modHeader.sample[i].length & 0xff) << 8) |
                                  (modHeader.sample[i].length >> 8));
    modHeader.sample[i].loopStart =
        (((modHeader.sample[i].loopStart & 0xff) << 8) |
         (modHeader.sample[i].loopStart >> 8));
    modHeader.sample[i].loopLength =
        (((modHeader.sample[i].loopLength & 0xff) << 8) |
         (modHeader.sample[i].loopLength >> 8));
  }

  // one byte stores number of orders
  // prevents reads of all orders higher than count+1
  fread(&modHeader.orderCount, 1, 1, modFile);
  u8 trash; // unused byte in mod format
  fread(&trash, 1, 1, modFile);

  // orders specify pattern to play, song plays orders sequentially
  fread(modHeader.order, 128, 1, modFile);
  u8 highestPattern = 0;
  for (i = 0; i < modHeader.orderCount; i++) {
    if (modHeader.order[i] > highestPattern) {
      highestPattern = modHeader.order[i];
    }
  }
  modHeader.patternCount = highestPattern + 1;

  // tutorial says we should be at position 1080, tested and it is

  // frequency = 7159090.5 / (period * 2);
  // these are unique to amiga cpu rate
  const u16 octave1Period[12] = {
      // this is the first row of the table from before
      856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453 // C-1 to B-1
  };

  u16 periodTable[60];
  u8 octave, note;

  for (octave = 0; octave < 5; octave++) {
    for (note = 0; note < 12; note++) {
      // *2 to get us into octave 0, then divide by 2 for each octave up from
      // there
      periodTable[octave * 12 + note] = (octave1Period[note] * 2) >> octave;
    }
  }

  modHeader.pattern = malloc(modHeader.patternCount * sizeof(u8 *));
  ASSERT(modHeader.pattern); // handle running out of memory
  memset(modHeader.pattern, 0, modHeader.patternCount * sizeof(u8 *));

  u8 currentPattern, row, column;
  for (currentPattern = 0; currentPattern < modHeader.patternCount;
       currentPattern++) {
    modHeader.pattern[currentPattern] = malloc(1024 * sizeof(u8));
    ASSERT(modHeader.pattern[currentPattern])
    memset(modHeader.pattern[currentPattern], 0, 1024);
  }

  for (row = 0; row < 64; row++) {
    for (column = 0; column < 4; column++) {
      u8 cell[4];
      u16 period;
      u8 sample;
      u8 effect;
      u8 param;

      fread(cell, 4, 1, modFile);
      sample = (cell[2] >> 4) | (cell[0] & 0xF0);
      period = cell[1] | ((cell[0] & 0x0F) << 8);
      effect = cell[2] >> 4;
      param = cell[3];

      u8 closestNote = 0;
      u16 closestDist = 0xFFFF;

      for (i = 0; i < 60; i++) {
        u16 newDist = abs(period - periodTable[i]);
        if (newDist < closestDist) {
          closestNote = i;
          closestDist = newDist;
        }
      }
      u8 *outCell =
          &modHeader.pattern[currentPattern][row * 4 * 4 + column * 4];
      outCell[0] = closestNote;
      outCell[1] = sample;
      outCell[2] = effect;
      outCell[3] = param;
    }
  }

  for (i = 0; i < 31; i++) {
    u32 realLength = ((u32)modHeader.sample[i].length) * 2;

    if (realLength != 0) {
    }
  }
  fclose(modFile);
  return 0;
}
