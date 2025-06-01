#include <stdio.h>
#define ROW_LENGTH 633
#define COL_HEIGHT 11 // once generator works, use scanf for w/h
#include "MiniGBA/MiniGBA.c"

  // Program creation steps:
  // Load in data table
  // Create empty 2d array
  // iterate through MiniGBABitmap, copying through all of Row Length, resetting to zero and COL_HEIGHT++ 

int main() {
  int arrangedTable[ROW_LENGTH][COL_HEIGHT];

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

