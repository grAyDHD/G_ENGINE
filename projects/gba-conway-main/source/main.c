//--------------------------------------------------------------------------------
// main.c
//--------------------------------------------------------------------------------
// The main routine
//--------------------------------------------------------------------------------
#include "utils/uintDigits.h"
#include "utils/xorshf96.h"
#include <tonc.h>

#include "assets/monogram.h"
#include "assets/starting.h"

void initScene(void);
void resetScene(BOOL random);
void updateScene(void);
void displayCycleCount(u32 cycles);

#define INTERRUPT_MASTER_ENABLE                                                \
  *(vu16 *)(0x04000208) //!< IRQ master enable, also REG_IME
#define IRQ_HANDLER *(fnptr *)(REG_BASE - 0x0004) //!< IRQ handler address
#define INTERRUPT_ENABLE                                                       \
  *(vu16 *)(REG_BASE + 0x0200) //!< IRQ enable, also REG_IE

//! IRQ Sender information
typedef struct IRQ_SENDER {
  u16 reg_ofs; //!< sender reg - REG_BASE
  u16 flag;    //!< irq-bit in sender reg
} ALIGN4 IRQ_SENDER;

// --------------------------------------------------------------------
// GLOBALS
// --------------------------------------------------------------------

// One extra entry for guaranteed zero
IRQ_REC __isr_table[II_MAX + 1];

// yeah, yeah, I really should use the registers and defines
// I have else where.
// NOTE: haven't really tested this very much; if inaccurate,
// plz tell me
static const IRQ_SENDER __irq_senders[] = {
    {0x0004, 0x0008}, // REG_DISPSTAT,	DSTAT_VBL_IRQ
    {0x0004, 0x0010}, // REG_DISPSTAT,	DSTAT_VHB_IRQ
    {0x0004, 0x0020}, // REG_DISPSTAT,	DSTAT_VCT_IRQ
    {0x0102, 0x0040}, // REG_TM0CNT,		TM_IRQ
    {0x0106, 0x0040}, // REG_TM1CNT,		TM_IRQ
    {0x010A, 0x0040}, // REG_TM2CNT,		TM_IRQ
    {0x010E, 0x0040}, // REG_TM3CNT,		TM_IRQ
    {0x0128, 0x4000}, // REG_SIOCNT		SIO_IRQ
    {0x00BA, 0x4000}, // REG_DMA0CNT_H,	DMA_IRQ>>16
    {0x00C6, 0x4000}, // REG_DMA1CNT_H,	DMA_IRQ>>16
    {0x00D2, 0x4000}, // REG_DMA2CNT_H,	DMA_IRQ>>16
    {0x00DE, 0x4000}, // REG_DMA3CNT_H,	DMA_IRQ>>16
    {0x0132, 0x4000}, // REG_KEYCNT,		KCNT_IRQ
    {0x0000, 0x0000}, // cart: none
};
// #define BIT(n)                               ( 1<<(n)
void IRQ_enable(enum eIrqIndex irq_id) {
  // u16 ime = REG_IME;
  // REG_IME = 0;

  u16 ime = INTERRUPT_MASTER_ENABLE;
  INTERRUPT_MASTER_ENABLE = 0;

  const IRQ_SENDER *sender = &__irq_senders[irq_id];
  *(u16 *)(REG_BASE + sender->reg_ofs) |= sender->flag;

  INTERRUPT_ENABLE |= BIT(irq_id);
  INTERRUPT_MASTER_ENABLE = ime;
}
// IWRAM_CODE void isr_master(void);
// IWRAM_CODE void isr_master_nest(void);
// extern IRQ_REC __isr_table[II_MAX+1];

int main() {
  // Set the ROM waitstates
  REG_WAITCNT = WS_SRAM_8 | WS_ROM0_N3 | WS_ROM0_S1 | WS_ROM1_N8 | WS_ROM1_S1 |
                WS_PREFETCH;

  //  IRQ_init(isr_master);
  INTERRUPT_MASTER_ENABLE = 0;
  memset32(__isr_table, 0, (II_MAX + 1) * sizeof(IRQ_REC) / 4);
  //__isr_table is the array of function pointers.
  // memset32 destination is this array.
  // src/value is 0, clearing all interrupt fnptrs
  // II_MAX+1 is number of fnptrs in the __isr_table
  // IRQ_REC is a struct with a flag (for interrupt source)
  // as well as a fnptr
  // sizeof(IRQ_REC) = 8 bytes.  divide by 4 = 2 words.
  // 2 words * number of fnptrs in __isr_table
  IRQ_HANDLER = (fnptr)isr_master;
  INTERRUPT_MASTER_ENABLE = 1;

  IRQ_enable(II_VBLANK);

  initScene();
  resetScene(FALSE);

  BOOL shouldUpdate = FALSE;

  while (1) {
    VBlankIntrWait();
    key_poll();

    if (shouldUpdate) {
      updateScene();

      if (key_hit(KEY_A | KEY_B | KEY_START))
        shouldUpdate = FALSE;

      if (key_hit(KEY_START))
        // No, just key_held(KEY_L) won't work, nor (BOOL)key_held(KEY_L).
        // Since BOOL is defined as u8, and key_held actually returns KEY_L
        // itself if the key is held, and KEY_L is outside the range of an u8,
        // this would never be true.
        resetScene(key_held(KEY_L) != 0);
    } else {
      if (key_hit(KEY_A))
        shouldUpdate = TRUE;
      else if (key_hit(KEY_B))
        updateScene();
    }

    // Call it to move the random seed
    (void)xorshf96();
  }
}

void initScene(void) {
  // Set the palette colors
  pal_bg_bank[0][0] = RGB15(0, 0, 12);
  pal_bg_bank[0][1] = RGB15(31, 31, 31);

  // Clear all the tiles that we'll need
  memset32(&tile_mem[0][0], 0, 660 * sizeof(TILE4) / sizeof(u32));
  memset32(&tile_mem[2][0], 0, 660 * sizeof(TILE4) / sizeof(u32));

  // Configure the screenblock with the desired layout
  // What we want is to set the tiles in a column-major way, such that the
  // bottom tile represents the tile index immediately after We will also leave
  // a "border" of one tile below and above to make space for the Conway
  // algorithm to work, since it will assume a "column-wise" orientation for the
  // tiles, like this: |   1 |  23 |  45 |  67 |  89 | ... | 639 | |   2 |  24 |
  // 46 |  68 |  90 | ... | 640 | |   3 |  25 |  47 |  69 |  91 | ... | 641 | |
  // 4 |  26 |  48 |  70 |  92 | ... | 642 | | ... | ... | ... | ... | ... | ...
  // | ... | |  20 |  42 |  64 |  86 | 108 | ... | 658 | (a column-major layout
  // is explained in tonc:
  // https://www.coranac.com/tonc/text/tte.htm#ssec-srf-format)
  for (u32 j = 0; j < 20; j++)
    for (u32 i = 0; i < 30; i++)
      se_mem[31][j * 32 + i] = 22 * i + j + 1;

  // Initialize all OAM sprites to hidden
  OAM_CLEAR();

  // Load the Monogram characters
  memcpy32(&tile_mem_obj[0][1], monogramTiles, monogramTilesLen / sizeof(u32));
}

int curScreen = 1;

void resetScene(BOOL random) {
  // Force a VBlank
  REG_DISPCNT = DCNT_BLANK;

  if (random) {
    // Populate the screen using randomness
    u32 lastRandom = 0;
    for (u32 i = 0; i < 30; i++)
      for (u32 j = 0; j < 20; j++)
        for (u32 k = 0; k < 8; k++) {
          if (k % 2 == 0)
            lastRandom = xorshf96();
          tile_mem[0][22 * i + j + 1].data[k] =
              (lastRandom >> 1) & lastRandom & 0x11111111;
        }
  } else {
    // Pre-populate the screen using starting tiles
    for (u32 i = 0; i < 30; i++)
      memcpy32(&tile_mem[0][22 * i + 1], startingTiles + (640 * i),
               640 / sizeof(u32));
  }

  // Wait for VBlank
  VBlankIntrWait();

  // Set the parameters for the initial scene
  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;

  // Configure background 0 to use SBB 31 and CBB 2 (it'll be switched to 0 on
  // the update)
  REG_BG0CNT = BG_4BPP | BG_SBB(31) | BG_REG_32x32;

  // Set the initial screen to 1 (to be flipped back to 0 on the update)
  curScreen = 1;

  oam_init(oam_mem, 24);
}

extern void conwaySimulationAdvance(const TILE *src, TILE *dest);

void updateScene(void) {
  // This is the screen "alternator" - even though it starts at 1, the first
  // iteration will flip it to 0
  curScreen = !curScreen;

  // Set up the background - this will flip through both screens
  if (curScreen == 1)
    REG_BG0CNT |= BG_CBB(2);
  else
    REG_BG0CNT &= ~BG_CBB(2);

  // Count the number of operations
  REG_TM0D = 0;
  REG_TM1D = 0;
  REG_TM1CNT = TM_CASCADE | TM_ENABLE;
  REG_TM0CNT = TM_ENABLE;

  // Kick off the Conway simulation
  conwaySimulationAdvance(&tile_mem[2 * curScreen][1],
                          &tile_mem[2 * (1 - curScreen)][1]);

  // Stop the timer and show the values
  REG_TM0CNT = 0;
  REG_TM1CNT = 0;
  displayCycleCount((REG_TM1D << 16) | REG_TM0D);
}

#define VBLANK_NUM_CYCLES 280896

void displayCycleCount(u32 cycles) {
  // Get the digits
  char buffer[12];
  char *end = uintDigits(buffer, cycles);
  uint numDigits = end - buffer;

  OBJ_ATTR *curObj = oam_mem;
  u32 curX = 4;

  // Display the numbers as sprites
  for (uint i = 0; i < numDigits; i++) {
    curObj->attr0 = ATTR0_Y(149);
    curObj->attr1 = ATTR1_X(curX + 6 * (numDigits - i - 1));
    curObj->attr2 = ATTR2_ID(1 + buffer[i]);
    curObj++;
  }
  curX += 6 * numDigits + 3;

  // Display the '('
  curObj->attr0 = ATTR0_Y(149);
  curObj->attr1 = ATTR1_X(curX);
  curObj->attr2 = ATTR2_ID(11);
  curObj++;
  curX += 3;

  // Display an approximate percentage
  u32 value = (cycles << 8) / VBLANK_NUM_CYCLES;
  u32 digits[3];
  digits[0] = value >> 8;
  value = (value - (digits[0] << 8)) * 10;
  digits[1] = value >> 8;
  value = (value - (digits[1] << 8)) * 10;
  digits[2] = value >> 8;

  // Chose which digit should be used
  u32 digitStart = digits[0] != 0 ? 0 : digits[1] != 0 ? 1 : 2;
  u32 numPercentDigits = 3 - digitStart;
  for (u32 i = 0; i < numPercentDigits; i++) {
    curObj->attr0 = ATTR0_Y(149);
    curObj->attr1 = ATTR1_X(curX);
    curObj->attr2 = ATTR2_ID(1 + digits[digitStart + i]);
    curObj++;
    curX += 6;
  }

  // Display the '%' and the ')'
  curObj->attr0 = ATTR0_Y(149);
  curObj->attr1 = ATTR1_X(curX);
  curObj->attr2 = ATTR2_ID(13);
  curObj++;
  curX += 6;

  curObj->attr0 = ATTR0_Y(149);
  curObj->attr1 = ATTR1_X(curX);
  curObj->attr2 = ATTR2_ID(12);
  curObj++;

  // Set the color
  if (cycles < 3 * VBLANK_NUM_CYCLES / 10)
    pal_obj_mem[1] = RGB15(0, 21, 0);
  else if (cycles < 5 * VBLANK_NUM_CYCLES / 10)
    pal_obj_mem[1] = RGB15(21, 21, 0);
  else
    pal_obj_mem[1] = RGB15(21, 0, 0);
}
