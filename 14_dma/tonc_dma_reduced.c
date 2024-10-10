
#include "brin.h" // Assuming brin.h includes your background data
#include <tonc.h>

void init_main() {
  // --- Load the background image data ---
  dma3_cpy(pal_bg_mem, brinPal, brinPalLen);      // Copy background palette
  dma3_cpy(tile_mem[0], brinTiles, brinTilesLen); // Copy background tiles
  dma3_cpy(se_mem[30], brinMap, brinMapLen);      // Copy background map

  // Set up BG2 (main background) to show the full image
  REG_BG2CNT = BG_CBB(0) | BG_SBB(30) |
               BG_REG_64x32; // Character & screen base, 64x32 map size
  REG_BG2HOFS = 0;           // No horizontal offset
  REG_BG2VOFS = 0;           // No vertical offset

  // Enable BG0, BG1, and BG2 for display
  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2;
}

int main() {
  init_main(); // Initialize background and display settings

  while (1) {
    vid_vsync(); // Wait for VBlank for smooth updates
  }

  return 0;
}
