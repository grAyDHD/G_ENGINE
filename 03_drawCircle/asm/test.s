.arm
.global main
.global RGB
.global drawPixel
.section .rodata  @ Read-only data section
DSPC:
  .word 0x4000000
VRAM:
  .word 0x6000000
M3BG2:
  .hword 0x403

.section .data  @ Initialized data section

.section .bss  @ Uninitialized data section

.section .text  @ Code section

main:
  ldr r8, =DSPC             @ Load base address for display control
  ldr r9, =M3BG2                 @ Set Mode 3 and enable BG2
  strh r9, [r8]                  @ Write to display control register

infin:
    b infin                       @ Infinite loop to keep the program running
