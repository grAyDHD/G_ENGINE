#include <stdio.h>
#include "MiniGBA/MiniGBA.c"

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


  /*
  // Prints table copy row by row, verified correct
  for (int i = 0; i < COL_HEIGHT; i++) {
    printf("Row %d:\n ", i);
    for (int j = 0; j < ROW_LENGTH; j++) {
      printf("%d: %x ", j, arrangedTable[i][j]);
    }
    printf("\n\n");
  }
  */

int main() {
  
  int arrangedTable[COL_HEIGHT][ROW_LENGTH];  // [row][column]
  for (int i = 0; i < COL_HEIGHT; i++) {      // j = row
    for (int j = 0; j < ROW_LENGTH; j++) {    // i = column  
      arrangedTable[i][j] = MiniGBABitmap[j + (i * STRIDE)];
    }
  }



  GlyphInfo fontData[94];
  int width = 1;
  int fontDataIndex = 0;
  int glyphStartX = 0;
  
  for (int i = 0; i < ROW_LENGTH; i++) { 
    int bgMatch = 0;
    for (int j = 0; j < COL_HEIGHT; j++) {
      if (arrangedTable[j][i] == 0x7C1F) {
        bgMatch++;
      }
    }

    if (bgMatch == COL_HEIGHT) {
        fontData[fontDataIndex].width = width;
        fontData[fontDataIndex].xOffset = (glyphStartX);
        fontDataIndex++;
        width = 0;
        glyphStartX = i + 1;
    } else {
      width++;
    }
  }

  for (int i = 0; i < 94; i++) {
    printf("fontData[%d]: { xOffset: %d, width: %d}\n ", i, fontData[i].xOffset, fontData[i].width);
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

