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
@  ldr r3, [r1, #8]                     @ r3 = state
@  cmp r3, #0                          @ if r3 = 0
@  beq Continue                        @ no more offset needed

@  cmp r3, #1                          @ if r3 = 1
@  bne Continue                        @ temporary break 
@  add r2, r2, r6                      @ add 128 pixel offset for walk state

  @ r0 = VRAM position offset
  @ r2 = image frame+direction+state offset
  @ r4 = frame byte width
  @ r6 = sprite sheet row length (pixels), also width and/or height of state offset in bytes
  @ TODO: calculate remaining state offsets




  @ Draw one row of sprite frame
  mov r3, #32                           @ Set up row counter
.LoopRow:
  ldmia r2!, {r4-r11}                   @ load pixels from image
  stmia r0!, {r4-r11}                   @ write pixels vram
  ldmia r2!, {r4-r11}                   @ load pixels from image
  stmia r0!, {r4-r11}                   @ write pixels vram

  add r2, #448                          @ move image offset to beginning of frame next row
  add r0, #416                          @ jump to beginning of next line VRAM, 480 - 64
  subs r3, #1                           @ decrement counter
  bne .LoopRow                          @ return to LoopRow until r3 = 1

  pop {r4-r11}
  bx lr 

.size SpriteFrame32Bit, .-SpriteFrame32Bit
