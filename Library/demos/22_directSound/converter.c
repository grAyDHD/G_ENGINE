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
  char songName[20]; // null terminate name[19]

  SAMPLE_HEADER sample[31];
  s8 *sampleData;  // stored at end of file
} MOD_HEADER;

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


  fclose(modFile);

  return 0;
}
