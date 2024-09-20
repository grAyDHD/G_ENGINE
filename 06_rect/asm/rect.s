.arm
.global main

.section .rodata @ Read-only data section
DSPC:
  .word 0x4000000
VRAM
  .word 0x6000000

M3BG2:
  .hword 0x403

green:
  .hword 0x03E0


.section .data @ initialized data section
sq_offset:
  .word 0x9680
sq_size:
  .word 20
y_offset:
  .word 9600

.section .text @ Code section
main:
  ldr r0, =DSPC 
  ldr r1, =M3BG2
  str r1, [r0]

  ldr r0, =VRAM
  ldr r1, =green
  ldr r2, =sq_offset  @square top left corner offset
  mov r3, sq_size
  mov r5, y_offset
  add r4, r0, r2
  add r6, r4, r5
draw_horizontal:
  strh r1, [r4], #2
  strh r1, [r6], #2 
  subs r3, r3, #1 
  bne draw_horizontal

  strh r1, [r4]
setup_v_draw:
  strh r1, [r4]
  add r4, r0, r2 @starting corner
  mov r3, #sq_size
  ldr r5, #y_offset

draw_vertical:
  strh r1, [r4]
  add r4, r4, r5 
  strh r1, [r6]
  sub r6, r6, r5
  
  subs r3, r3, #1
  bne draw_vertical
infin:
  b infin


