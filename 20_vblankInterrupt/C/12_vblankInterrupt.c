#include <engine.h>
//! IRQ Sender information
#define ALIGN4 __attribute__((aligned(4)))
#define BIT(n) (1 << (n))

#define REG_BASE 0x04000000
//!< IRQ master enable
#define REG_IME *(volatile unsigned short *)(REG_BASE + 0x0208)
// IRQ enable
#define REG_IE *(volatile unsigned short *)(REG_BASE + 0x0200)
// IRQ handler address
#define REG_ISR_MAIN *(fnptr *)(REG_BASE - 0x0004)

typedef struct IRQ_SENDER {
  u16 reg_ofs; //!< sender reg - REG_BASE
  u16 flag;    //!< irq-bit in sender reg
} ALIGN4 IRQ_SENDER;

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

typedef void (*fnptr)(void); //!< void foo() function pointer
typedef struct IRQ_REC {
  u32 flag;  //!< Flag for interrupt in REG_IF, etc
  fnptr isr; //!< Pointer to interrupt routine
} IRQ_REC;

// One extra entry for guaranteed zero
IRQ_REC __isr_table[II_MAX + 1];

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

void IRQ_init(fnptr isr) {

  REG_IME = 0;
  // clear interrupt table (just in case)
  // I understand how memset works, taking src, destination, and size of data as
  // word count. I have implemented custom memset and memcpy asm functions
  memset32(__isr_table, 0, (II_MAX + 1) * sizeof(IRQ_REC) / 4);
  REG_ISR_MAIN = (isr) ? isr : (fnptr)isr_master;
  REG_IME = 1;
}

void IRQ_enable(enum eIrqIndex irq_id) {
  u16 ime = REG_IME;
  REG_IME = 0;
  const IRQ_SENDER *sender = &__irq_senders[irq_id];
  *(u16 *)(REG_BASE + sender->reg_ofs) |= sender->flag;
  REG_IE |= BIT(irq_id);
  REG_IME = ime;
}

int main() {

  IRQ_init(isr_master); // isr_master defines the __isr_table
  IRQ_enable(II_VBLANK);

  return 0;
}
