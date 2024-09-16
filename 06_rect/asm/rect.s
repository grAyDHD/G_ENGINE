.arm
.text
.global main

main:
  ldr r0, =0x4000000 
  ldr r1, =0x403
  str r1, [r0] @set mode 3, bg2

  ldr r0, =0x6000000
  ldr r1, =0x03E0  @green
  ldr r2, =0x9680  @square top left corner offset
  mov r3, #20 @square height/width, used as counter
  mov r5, #9600 @y offset
  add r4, r0, r2 @add corner offset to base vram
  add r6, r4, r5
draw_horizontal:
  strh r1, [r4], #2
  strh r1, [r6], #2 
  subs r3, r3, #1 
  bne draw_horizontal

setup_v_draw:
  strh r1, [r4]
  add r4, r0, r2 @starting corner
  mov r3, #20
  mov r5, #240
  add r5, r5, r5

draw_vertical:
  strh r1, [r4]
  add r4, r4, r5 
  strh r1, [r6]
  sub r6, r6, r5
  
  subs r3, r3, #1
  bne draw_vertical
infin:
  b infin


