#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef int32_t s32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef int8_t s8;

//songFreq = tempo*2/5;
//

#define ASSERT(condition) \
  if (!(condition)) { \
    printf("ERROR: %s failed at line %d\n", #condition, __LINE__); \
    exit(1); \
  }

typedef struct __attribute__((packed)){
  char name[22];
  u16 length;
  u8 finetune;
  u8 vol;
  u16 loopStart; //swap bytes, word count (2 byte words)
  u16 loopLength;//swap bytes, word count (2 byte words)

} SAMPLE_HEADER;

typedef struct {
  char signature[5];
  char songName[20];

  SAMPLE_HEADER sample[31];

  u8 order[128];
  u8 orderCount;
  u8 **pattern;
  u8 patternCount; 

  s8 *sampleData;  // stored at end of file
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

 
    modHeader.sample[i].length = (((modHeader.sample[i].length & 0xff) << 8) | (modHeader.sample[i].length >> 8));
    modHeader.sample[i].loopStart =  (((modHeader.sample[i].loopStart & 0xff) << 8) | (modHeader.sample[i].loopStart >> 8));
    modHeader.sample[i].loopLength = (((modHeader.sample[i].loopLength & 0xff) << 8) | (modHeader.sample[i].loopLength >> 8));
}

  fread(&modHeader.orderCount, 1, 1, modFile);

  u8 trash;
  fread(&trash, 1, 1, modFile);

  fread(modHeader.order, 128, 1, modFile);

  u8 highestPattern = 0;

  for (i = 0; i < modHeader.orderCount; i++) {
    if (modHeader.order[i] > highestPattern) {
      highestPattern = modHeader.order[i];
    }
  }

  modHeader.patternCount = highestPattern + 1;
  
  // tutorial says we should be at position 1080, tested and it is
  //
  // pattern sort of like bitmap
  // 4 channels, 4 columns wide
  // all MOD patterns have 64 rows
  // 4x64, 4 bytes per cell, 1024 bytes or 1 KB
  // each cell contains 4 fields:
  //  8-bit sample number,
  //  4-bit effect type
  //  4-bit effect parameter
  
  u8 cell[4];
  u8 sample;
  u16 period;
  u8 effect;
  u8 param;

  fread(cell, 4, 1, modFile);

  sample = (cell[2] >> 4) | (cell[0] & 0xF0);
  period = cell[1] | ((cell[0] & 0xF) << 8);
  effect = cell[2] & 0xF;
  param = cell[3];

/*
frequency = 7159090.5 / (428 * 2);
frequency = 8363.42;

Remember earlier I said 8363 is the default
 *
  dc.w 856,808,762,720,678,640,604,570,538,508,480,453 ; C-1 to B-1
	dc.w 428,404,381,360,339,320,302,285,269,254,240,226 ; C-2 to B-2
	dc.w 214,202,190,180,170,160,151,143,135,127,120,113 ; C-3 to B-3
 *
 * */
const u16 octave1Period[12] = {
      // this is the first row of the table from before
   856,808,762,720,678,640,604,570,538,508,480,453   // C-1 to B-1
};

u16 periodTable[12*5];
u8 octave, note;

for(octave = 0; octave < 5; octave++)
{
   for(note = 0; note < 12; note++)
   {
         // *2 to get us into octave 0, then divide by 2 for each octave up from there
      periodTable[octave*12 + note] = (octave1Period[note]*2) >> octave;
   }
}


  /* PATTERN DATA
   *  note value: 6 bits
   *  sample number: 5 bits (stored as byte)
   *  effect: 4 bits
   *  param: 8 bits
   *  23 bits total.
   *  
   byte0    byte1     byte2  
---------- -------- ----------
| NNNNNNss SSSxEEEE FFFFFFFF |
---------- -------- ----------
bit0.....7,0......7,0......7

Note:   NNNNNN
Sample: ssSSS
Effect: EEEE
Param:  FFFFFFFF
x: unused
   *
   *


   byte0    byte1     byte2    byte3
---------- -------- -------- ----------
| NNNNNNxx SSSSSxxx EEEExxxx FFFFFFFF |
---------- -------- -------- ----------
bit0.....7,0......7,0......7,0......7
   */
   // 5 octaves' worth of space



modHeader.pattern = new u8*[modHeader.patternCount];
ASSERT( modHeader.pattern );   // handle running out of memory
   // Initialize the memory to 0
memset( modHeader.pattern, 0, modHeader.patternCount*sizeof(u8*) );

u8 curPattern, row, column;

for(curPattern = 0; curPattern < modHeader.patternCount; curPattern++);
{
      // Allocate the memory for our new pattern (they are always 1K)
   modHeader.pattern[curPattern] = new u8[1024];
   ASSERT( modHeader.pattern[curPattern] );   // handle running out of memory
   memset( modHeader.pattern[curPattern], 0, 1024);   // initialize to 0

   for(row = 0; row < 64; row++)
   {
      for(column = 0; column < 4; column++)
      {
///////////////////////////// Copy our bit of code from earlier
         u8 cell[4];
         u8 sample;
         u16 period;
         u8 effect;
         u8 param;

         fread(cell, 4, 1, modFile);

         sample = (cell[0] & 0xF0) | (cell[2] >> 4);
         period = cell[1] | ((cell[0] & 0xF) << 8);
         effect = cell[2] & 0xF;
         param  = cell[3];
//////////////////////////// end copied code

            // Now we'll loop through our note period table and find the closest match
         u8 closestNote = 0;
         u16 closestDist = 0xffff;   // make sure the first comparison sets the closest note

         for(i = 0; i < 12*5; i++)   // 5 octaves, 12 notes each
         {
            u16 newDist = abs( period - periodTable[i] );
            if(newDist < closestDist)
            {
               closestNote = i;
               closestDist = newDist;
            }
         }

            // Now that we have our note, we can store the data in our new pattern
            // Calculate the address of the cell to output to
            // rowOffset = row * 4 columns per row * 4 bytes per cell
            // columnOffset = column * 4 bytes per cell
         u8 *outCell = &modHeader.pattern[curPattern][row*4*4 + column*4];
         outCell[0] = closestNote;
         outCell[1] = sample;
         outCell[2] = effect;
         outCell[3] = param;
      }
   }
}


for(i = 0; i < 31; i++)
{
      // Compute the real length in bytes
   u32 realLength = ((u32)modHeader.sample[i].length) * 2;

   if(realLength != 0)
   {
      modHeader.sample[i].smpData = new s8[realLength];
      ASSERT(modHeader.sample[i].smpData != NULL);

      fread(modHeader.sample[i].smpData, realLength, 1, modFile);
   }
}
  fclose(modFile);
  return 0;
}
