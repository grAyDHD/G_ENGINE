//! Interrups Indices
typedef enum eIrqIndex {
  II_VBLANK = 0,
  II_HBLANK,
  II_VCOUNT,
  II_TIMER0,
  II_TIMER1,
  II_TIMER2,
  II_TIMER3,
  II_SERIAL,
  II_DMA0,
  II_DMA1,
  II_DMA2,
  II_DMA3,
  II_KEYPAD,
  II_GAMEPAK,
  II_MAX
} eIrqIndex;

//! Struct for prioritized irq table
typedef struct IRQ_REC {
  u32 flag;  //!< Flag for interrupt in REG_IF, etc
  fnptr isr; //!< Pointer to interrupt routine
} IRQ_REC;

// === PROTOTYPES =====================================================

IWRAM_CODE void isr_master_nest();

void irq_init(fnptr isr);
fnptr irq_set_master(fnptr isr);

fnptr irq_add(enum eIrqIndex irq_id, fnptr isr);
fnptr irq_delete(enum eIrqIndex irq_id);

fnptr irq_set(enum eIrqIndex irq_id, fnptr isr, int prio);
void irq_enable(enum eIrqIndex irq_id);
void irq_disable(enum eIrqIndex irq_id);
// IRQ Sender information
typedef struct IRQ_SENDER {
  u16 reg_ofs; //!< sender reg - REG_BASE
  u16 flag;    //!< irq-bit in sender reg
} ALIGN4 IRQ_SENDER;

// === GLOBALS ========================================================

// One extra entry for guaranteed zero
IRQ_REC __isr_table[II_MAX + 1];

static const IRQ_SENDER __irq_senders[] = {
    {0x0004, 0x0008}, // REG_DISPSTAT,    DSTAT_VBL_IRQ
    {0x0004, 0x0010}, // REG_DISPSTAT,    DSTAT_VHB_IRQ
    {0x0004, 0x0020}, // REG_DISPSTAT,    DSTAT_VCT_IRQ
    {0x0102, 0x0040}, // REG_TM0CNT,      TM_IRQ
    {0x0106, 0x0040}, // REG_TM1CNT,      TM_IRQ
    {0x010A, 0x0040}, // REG_TM2CNT,      TM_IRQ
    {0x010E, 0x0040}, // REG_TM3CNT,      TM_IRQ
    {0x0128, 0x4000}, // REG_SCCNT_L      BIT(14) // not sure
    {0x00BA, 0x4000}, // REG_DMA0CNT_H,   DMA_IRQ>>16
    {0x00C6, 0x4000}, // REG_DMA1CNT_H,   DMA_IRQ>>16
    {0x00D2, 0x4000}, // REG_DMA2CNT_H,   DMA_IRQ>>16
    {0x00DE, 0x4000}, // REG_DMA3CNT_H,   DMA_IRQ>>16
    {0x0132, 0x4000}, // REG_KEYCNT,      KCNT_IRQ
    {0x0000, 0x0000}, // cart: none
};

// === FUNCTIONS ======================================================

//! Enable irq bits in REG_IE and sender bits elsewhere
void irq_enable(enum eIrqIndex irq_id) {
  u16 ime = REG_IME;
  REG_IME = 0;

  const IRQ_SENDER *sender = &__irq_senders[irq_id];
  *(u16 *)(REG_BASE + sender->reg_ofs) |= sender->flag;

  REG_IE |= BIT(irq_id);
  REG_IME = ime;
}

//! Add a specific isr
fnptr irq_add(enum eIrqIndex irq_id, fnptr isr) {
  u16 ime = REG_IME;
  REG_IME = 0;

  int ii;
  u16 irq_flag = BIT(irq_id);
  fnptr old_isr;
  IRQ_REC *pir = __isr_table;

  // Enable irq
  const IRQ_SENDER *sender = &__irq_senders[irq_id];
  *(u16 *)(REG_BASE + sender->reg_ofs) |= sender->flag;
  REG_IE |= irq_flag;

  // Search for previous occurance, or empty slot
  for (ii = 0; pir[ii].flag; ii++)
    if (pir[ii].flag == irq_flag)
      break;

  old_isr = pir[ii].isr;
  pir[ii].isr = isr;
  pir[ii].flag = irq_flag;

  REG_IME = ime;
  return old_isr;
}

// This is mostly what tonclib's isr_master does, but
// you really need asm for the full functionality
IWRAM_CODE void isr_master_c() {
  u32 ie = REG_IE;
  u32 ieif = ie & REG_IF;
  IRQ_REC *pir;

  // (1) Acknowledge IRQ for hardware and BIOS.
  REG_IF = ieif;
  REG_IFBIOS |= ieif;

  // (2) Find raised irq
  for (pir = __isr_table; pir->flag != 0; pir++)
    if (pir->flag & ieif)
      break;

  // (3) Just return if irq not found in list or has no isr.
  if (pir->flag == 0 || pir->isr == NULL)
    return;

  // --- If we're here have an interrupt routine ---
  // (4a) Disable IME and clear the current IRQ in IE
  u32 ime = REG_IME;
  REG_IME = 0;
  REG_IE &= ~ieif;

  // (5a) CPU back to system mode
  //> *(--sp_irq)= lr_irq;
  //> *(--sp_irq)= spsr
  //> cpsr &= ~(CPU_MODE_MASK | CPU_IRQ_OFF);
  //> cpsr |= CPU_MODE_SYS;
  //> *(--sp_sys) = lr_sys;

  pir->isr(); // (6) Run the ISR

  REG_IME = 0; // Clear IME again (safety)

  // (5b) Back to irq mode
  //> lr_sys = *sp_sys++;
  //> cpsr &= ~(CPU_MODE_MASK | CPU_IRQ_OFF);
  //> cpsr |= CPU_MODE_IRQ | CPU_IRQ_OFF;
  //> spsr = *sp_irq++
  //> lr_irq = *sp_irq++;

  // (4b) Restore original ie and ime
  REG_IE = ie;
  REG_IME = ime;
}

#include <stdio.h>
#include <tonc.h>

IWRAM_CODE void isr_master();
IWRAM_CODE void hbl_grad_direct();

void vct_wait();
void vct_wait_nest();

CSTR strings[] = {"asm/nested", "c/direct", "HBlank", "VCount"};

// Function pointers to master isrs.
const fnptr master_isrs[2] = {(fnptr)isr_master, (fnptr)hbl_grad_direct};

// VCount interrupt routines.
const fnptr vct_isrs[2] = {vct_wait, vct_wait_nest};

// (1) Uses tonc_isr_master.s' isr_master() as a switchboard
void hbl_grad_routed() {
  u32 clr = REG_VCOUNT / 8;
  pal_bg_mem[0] = RGB15(clr, 0, 31 - clr);
}

// (2a) VCT is triggered at line 80; this waits 40 scanlines
void vct_wait() {
  pal_bg_mem[0] = CLR_RED;
  while (REG_VCOUNT < 120)
    ;
}

// (2b) As vct_wait(), but interruptable by HBlank
void vct_wait_nest() {
  pal_bg_mem[0] = CLR_RED;
  REG_IE = IRQ_HBLANK; // Allow nested hblanks
  REG_IME = 1;
  while (REG_VCOUNT < 120)
    ;
}

int main() {
  u32 bDirect = 0, bVctPrio = 0;

  tte_init_chr4_b4_default(0, BG_CBB(2) | BG_SBB(28));
  tte_set_drawg((fnDrawg)chr4_drawg_b4cts_fast);
  tte_init_con();
  tte_set_margins(8, 8, 128, 64);

  REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;

  // (3) Initialize irqs; add HBL and VCT isrs
  // and set VCT to trigger at 80
  irq_init(master_isrs[0]);
  irq_add(II_HBLANK, hbl_grad_routed);
  BFN_SET(REG_DISPSTAT, 80, DSTAT_VCT);
  irq_add(II_VCOUNT, vct_wait);
  irq_add(II_VBLANK, NULL);

  while (1) {
    // vid_vsync();
    VBlankIntrWait();
    key_poll();

    // Toggle HBlank irq
    if (key_hit(KEY_R))
      REG_IE ^= IRQ_HBLANK;

    // Toggle Vcount irq
    if (key_hit(KEY_L))
      REG_IE ^= IRQ_VCOUNT;

    // (4) Toggle between
    // asm switchblock + hbl_gradient (red, descending)
    // or purely hbl_isr_in_c (green, ascending)
    if (key_hit(KEY_A)) {
      bDirect ^= 1;
      irq_set_master(master_isrs[bDirect]);
    }

    // (5) Switch priorities of HBlank and VCount
    if (key_hit(KEY_B)) {
      // irq_set(II_VCOUNT, vct_wait, bVctPrio);
      bVctPrio ^= 1;
      irq_add(II_VCOUNT, vct_isrs[bVctPrio]);
    }

    tte_printf("#{es;P}IRS#{X:32}: %s\nPrio#{X:32}: %s\nIE#{X:32}: %04X",
               strings[bDirect], strings[2 + bVctPrio], REG_IE);
  }

  return 0;
}
int main() { return 0; }

The master interrupt routines have to be ARM code. As we've always compiled to THUMB code, this would be something new. The reason that we've always compiled to THUMB code is that the 16bit buses of the normal code sections make ARM-code slow there. However, what we could do is put the ARM code in IWRAM, which has a 32bit bus (and no waitstates) so that it's actually beneficial to use ARM code there.

Compiling as ARM code is actually quite simple: use -marm instead of -mthumb. The IWRAM part is what causes the most problems. There are GCC extensions that let you specify which section a function should be in. Tonclib has the following macros for them:

#define EWRAM_DATA __attribute__((section(".ewram")))
#define IWRAM_DATA __attribute__((section(".iwram")))
#define EWRAM_BSS __attribute__((section(".sbss")))

#define EWRAM_CODE __attribute__((section(".ewram"), long_call))
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))


// --- Examples of use: ---
// Declarations
extern EWRAM_DATA u8 data[];
IWRAM_CODE void foo();

// Definitions
EWRAM_DATA u8 data[8] = {...};

IWRAM_CODE void foo(){....}

#include <tonc.h>

IWRAM_CODE void hbl_grad_direct();

// an interrupt routine purely in C
// (make SURE you compile in ARM mode!!)
void hbl_grad_direct() {
  u32 irqs = REG_IF & REG_IE;

  REG_IFBIOS |= irqs;
  if (irqs & IRQ_HBLANK) {
    u32 clr = REG_VCOUNT / 8;
    pal_bg_mem[0] = RGB15(0, clr, 0);
  }

  REG_IF = irqs;
}

lags for ARM+IWRAM compilation
Replace the ‘-mthumb’ in your compilation flags by ‘-marm -mlong-calls’. For example:

CBASE   := $(INCDIR) -O2 -Wall

#ROM flags
RCFLAGS := $(CBASE) -mthumb-interwork -mthumb
#IWRAM flags
ICFLAGS := $(CBASE) -mthumb-interwork -marm -mlong-calls
