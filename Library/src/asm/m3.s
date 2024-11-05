.section .iwram, "ax", %progbits
.align 2

.global fillScreen
.arm
.type fillScreen, %function

fillScreen:
  @ r0 contains color value
  push {r4-r11}
  ldr r1, =0x6000000 @vram base address
  mov r2, #1920 @number of total writes to fill screen
  @ store color value in 9 more registers
  mov r3, r0
  mov r4, r0
  mov r5, r0
  mov r6, r0
  mov r7, r0
  mov r8, r0
  mov r9, r0
  mov r10, r0
  mov r11, r0
.Lscreen_write_32:
  stmia r1!, {r0, r3-r11}
  subs r2, r2, #1
  bhi .Lscreen_write_32
  pop {r4-r11}
  bx lr

  .size fillScreen, .-fillScreen
