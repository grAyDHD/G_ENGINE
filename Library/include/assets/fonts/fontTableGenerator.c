#include <stdio.h>
#include "MiniGBA/MiniGBA.c"
//#include "../../graphics/text.h"

typedef unsigned short u16;

typedef struct {
  u16 xOffset;
  u16 width;
} GlyphInfo;

#define ROW_LENGTH 633
#define COL_HEIGHT 11 
#define STRIDE (ROW_LENGTH + 1)
  // once generator works, use scanf for w/h?

  // Program creation steps:
  // Load in data table
  // Create empty 2d array
  // iterate through MiniGBABitmap, copying through all of Row Length, resetting to zero and COL_HEIGHT++ 
  // create fontDataTable
  // scan bitmap copy column by column UNTIL finding column where EVERY value = 0x7C1F

int main() {
  
  int arrangedTable[COL_HEIGHT][ROW_LENGTH];  // [row][column]
  for (int j = 0; j < COL_HEIGHT; j++) {      // j = row
    for (int i = 0; i < ROW_LENGTH; i++) {    // i = column  
      arrangedTable[j][i] = MiniGBABitmap[i + (j * STRIDE)];
    }
  }

  /*
  int arrangedTable[ROW_LENGTH][COL_HEIGHT];
  for (int i = 0; i < ROW_LENGTH; i++) {
    for (int j = 0; j < COL_HEIGHT; j++) {
      arrangedTable[i][j] = MiniGBABitmap[i + (j * ROW_LENGTH)];
    }
  }
  */

  // Prints table copy row by row, verified correct
  for (int j = 0; j < COL_HEIGHT; j++) {
    printf("Row %d:\n ", j);
    for (int i = 0; i < ROW_LENGTH; i++) {
      printf("%d: %x ", i, arrangedTable[j][i]);
    }
    printf("\n\n");

  }

  
  GlyphInfo fontData[94];
  int width = 1;
  int fontDataIndex = 0;
  for (int i = 0; i < ROW_LENGTH; i++) { 
    int bgMatch = 0;
    for (int j = 0; j < COL_HEIGHT; j++) {
      if (arrangedTable[i][j] == 0x7C1F) {
        bgMatch++;
      }
  
     if (bgMatch == COL_HEIGHT) {
        fontData[fontDataIndex].width = width;
        fontData[fontDataIndex].xOffset = (i - width);
        fontDataIndex++;
        width = 0;
      }

    }
    width++;
  }

  for (int i = 0; i < 94; i++) {
//    printf("fontData[%d]: { xOffset: %d, width: %d}\n ", i, fontData[i].xOffset, fontData[i].width);
  }

  return 0;
}

  // Goal of program:
  // create data table used for rendering glyphs from print functions
  
  // Program flow:
  // run ./fontTableGenerator FontName
  // program searches FontName directory for FontName.c
  // imports array in FontName.c
  // processes array into 2d array of 633 columns and 11 rows
  // creates fontData file with unpopulated array, or initialized to zero, whichever makes more sense/is easier
  // then parses through column by column to begin process of populating fontData array
  // saves changes to file once array is fully populated

