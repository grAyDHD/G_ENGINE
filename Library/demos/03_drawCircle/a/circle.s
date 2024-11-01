.arm
.global main
.global RGB
.global drawPixel

.section .text  @ Code section

main:
  ldr r8, =0x4000000            @ Load base address for display control
  ldr r9, =0x403                @ Set Mode 3 and enable BG2
  strh r9, [r8]                 @ Write to display control register

  ldr r8, =0x6000000            @ Load base address for VRAM 

  mov r0, #0
  mov r1, #0
  mov r2, #20
  bl RGB                       @ Calculates color value into r9

  mov r3, #120          @ Center of circle x 120
  mov r4, #80           @ Center of circle y #80
  mov r5, #40           @ Radius 40
                        @ use r6 as counter for s
                        @ r7, decisionOver2 = 1 - r
  bl drawCircle         @not working, screen fills eentirely with repeating diagonal lines
  b infin
  
RGB:    @ r0=r r1=g r2=b 0-31 each
  lsl r0, r0, #10
  lsl r1, r1, #5
  orr r0, r0, r1
  orr r9, r0, r2
  bx lr

drawPixel: @ r0 x, r1 y
  @write color value to vram offset calculated by ((y*240) +x) 
  mov r2, #240      @240
  mul r7, r1, r2    @ y*240
  add r0, r0, r7    @ x+(y*240)
  orr r0, r0, r8    @ add vram and offset
  strh r9, [r0]     @ store r9 color value at vram offset address at r0
  bx lr


@ x r3 y r4 r r5 s r6
@ r0, r1, x, y octant calcs.  r2 yoffset.  r3, r4, r5, r6, x,y,r,s. r7 mult.
@ r8 vram, r9 color.  r10 decisionOver2, r11.
drawCircle:
  //Need to loop the following logic while r >= s
  //if r=s, continue, as s increments by 1
  mov r6, #0      @reset s to zero 
  rsb r10, r5, #1 @init decisionOver2
  //plot octant one, x+r, y+s
  add r0, r3, r5
  add r1, r4, r6
  bl drawPixel
  //plot octant two, x+s y+r
  add r0, r3, r6
  add r0, r4, r5
  bl drawPixel
  //plot octant 3, x-s, y+r
  sub r0, r3, r6
  add r1, r4, r5
  bl drawPixel
  //plot octant 4, x-r, y+s
  sub r0, r3, r5
  add r1, r3, r6
  bl drawPixel
  //plot octant 5, x-r, y-s
  sub r0, r3, r5
  sub r1, r4, r6
  bl drawPixel
  //plot octant 6, x-s, y-r
  sub r0, r3, r6
  sub r1, r4, r5
  bl drawPixel
  //plot octant 7, x+s, y-s
  add r0, r3, r6
  sub r1, r4, r6
  bl drawPixel
  //plot octant 8, x+r, y-r
  add r0, r3, r5
  sub r1, r4, r5
  bl drawPixel
  // increment s
  add r6, #1
  //if decisionOver2 <= 0, move down in y, decisionOver2 += 2 * s + 1
  lsl r6, r6, #1  @ 2 * s
  add r6, #1     @ +1
  add r10, r10, r6
  //else r--, move down in y and x, decisionOver2 +=2 * (s-r) +1
  sub r5, #1  @decrement r by 1
  sub r6, r6, r5  @ s-r
  lsl r6, r6, #1
  adds r6, #1
  bne drawCircle
  bx lr
infin:
    b infin                       @ Infinite loop to keep the program running
