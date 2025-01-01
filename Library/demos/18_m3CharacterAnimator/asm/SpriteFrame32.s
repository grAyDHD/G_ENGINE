.arm
.global SpriteFrame32Bit
.type SpriteFrame32Bit, %function

@ 4 state, 4 direction, 4 frame animations

@ SpriteFrame32Bit(*image);
@ r0 = PositionComponent struct {int x, int y}
@ r1 = AnimationComponent struct {frame, direction, state}
@ r2 = image (pointer to sprite sheet) 

SpriteFrame32Bit:
  push {r4-r11}  @8x4 = 32 bytes?

@ Step 1: calculate VRAM xy offset address

  ldr r3, [r0]                          @ r3 = position.x
  ldr r4, [r0, #4]                      @ r4 = position.y
  mov r5, #240                          @ r5 = y offset multiplier
  mul r0, r4, r5                        @ r0 = y * 240
  add r3, r0, r3                        @ r4 = y offset + x

  ldr r0, =0x6000000  
  add r0, r0, r3
  add r0, r0, r3

  @ r0 = VRAM offset
  @ r1 AnimationComponent
  @ r2 image

@ Step 2: calculate image animation offset

@ calculate frame offset 
  ldr r3, [r1]                            @ r3 = frameNumber
  mov r4, #64                            @ r4 = frame width in bytes, 32 * 2
  mul r5, r3, r4                         @ r5 = frameOffset
  add r2, r2, r5                         @ r2 = image frame offset

@ Calculate direction offset
  ldr r3, [r1, #4]                         @ r3 = direction, 0 = down, 1 = up, 2 = left, 3 = right 
  ldr r6, =256                            @ r6 = pixel column offset for new direction frame
  mul r5, r3, r6                          @ r5 = direction * column offset
  mul r7, r5, r4                          @ r7 = final direction offset, r5 * 64 (r4)
  add r2, r2, r7                          @ r2 = image frame+direction offset
 
@ Calculate state offset for IDLE and WALK
  ldr r3, [r1, #8]                     @ r3 = state
  cmp r3, #0                          @ if r3 = 0
  beq .Continue                        @ no more offset needed

  cmp r3, #1                          @ if r3 = 1
  bne .Continue                        @ temporary break 
  add r2, r2, r6                      @ add 128 pixel offset for walk state

  @ r0 = VRAM position offset
  @ r2 = image frame+direction+state offset
  @ TODO: calculate remaining state offsets
.Continue:

@ Step 3: copy image to vram (not transparency pixels)
  mov r1, #32           @ Set up row counter
  ldr r3, =0x7C1F7C1F   @ both transparent 
  ldr r4, =0x00007C1F   @ left pixel trans mask
  ldr r5, =0x7C1F0000   @ right pixel trans mask
  ldr r6, =0x0000FFFF   @ clear right pixel mask 
  ldr r7, =0xFFFF0000   @ clear left pixel mask

.LoopRow:
  mov r11, #16
.PixelLoop:
  ldr r8, [r2], #4          @ r8 = image pixels
  cmp r8, r3                @ if both are tranparency color, skip storing anything to vram 
  beq .NextPixel

  ldr r9, [r0]  // save copy of vram in r9
  @ check right pixel
  and r10, r8, r7  // copy of image pixels with left cleared
  cmp r10, r5
  moveq r10, r9  //overwrite with vram
  and r10, r10, r7  // final value for right pixel regardless of image or vram

  @ check left pixel
  and r12, r8, r6 // copy of img pixels with right cleared
  cmp r12, r4 // compare left pixel to transparency value
  moveq r12, r9 //overwite with vram if left is transparent
  and r12, r12, r6 //clear right pixel, whether image or vram

  @ combine left and right pixels
  orr r9, r10, r12 // combine saved left and right final pixels
  str r9, [r0]  //store combined pixels in vram

.NextPixel:
  add r0, #4 @increment vram pointer
  subs r11, #1 @decrement pixel pair counter
  bne .PixelLoop
  
  add r2, #448                          @ move image offset to beginning of frame next row
  add r0, #416                          @ jump to beginning of next line VRAM, 480 - 64

  subs r1, #1                           @ decrement counter
  bne .LoopRow                          @ return to LoopRow until r3 = 1

  pop {r4-r11}
  bx lr 

.size SpriteFrame32Bit, .-SpriteFrame32Bit

  //now, need to check if either both or neither pixel is transparent.
@             10 11 12 13 14 15
@  0123456789 a  b  c  d  e  f

@     0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
@  0000 0001 0010 0011 0100 0101 0110 0111 1000 1001 1010 1011 1100 1101 1110 1111
  
@x    7    C    1    F       7    C    1    F
@  0111 1100 0001 1111    0111 1100 0001 1111

@x    7    C    1    F       0    0    0    0
@  0111 1100 0001 1111    0000 0000 0000 0000

@x    0    0    0    0       7    C    1    F
@  0000 0000 0000 0000    0111 1100 0001 1111


  //ldmia r2!, {r4-r11}                   @ load pixels from image
  //stmia r0!, {r4-r11}                   @ write pixels vram
  //ldmia r2!, {r4-r11}                   @ load pixels from image
  //stmia r0!, {r4-r11}                   @ write pixels vram

