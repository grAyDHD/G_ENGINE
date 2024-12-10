.arm
.global SpriteFrame
.type SpriteFrame, %function

@ Currently handles 4 frame animations of 16x16 sprite size

@ SpriteFrame(int x, int y, int frame, const void *image, int frameCount)
@ r0 = x
@ r1 = y
@ r2 = frame
@ r3 = image (pointer to sprite sheet)

SpriteFrame:
  push {r4-r11}  @8x4 = 32 bytes, lr on stack so 36 bytes + 4 for alignment
  
@ Step 1: calculate VRAM xy offset address
  mov r5, #240                           @ y base offset value
  mul r6, r1, r5                         @ y * 240
  add r0, r0, r6                         @ x+y*240
  add r0, r0, r0                         @ double
  ldr r1, =0x6000000
  add r1, r1, r0

                  @ r0 = x (no longer needed)
                  @ r1 = vram xy address 
                  @ r2 = frame 
                  @ r3 = image
 
@ Step 2: Calculate frame offset
  mov r4, #32                            @ 16 pixels = 32 bytes per row of a frame
  mul r5, r2, r4                         @ multiply frame number by frame width
  add r0, r3, r5                         @ add frame offset to image base address

                  @ r0 = image frame offset
                  @ r1 = vram xy address
                  @ r2 = frame (no longer needed)
                  @ r3 = image (no longer needed)
                  @ r4 = 32 length of row in bytes, 16px*2

@ Step 3: Loop through each row of the sprite (16 rows)
  ldr r2, [sp, #32]                     @ load 5th argument from stack 
  mul r3, r2, r4                        @ frame count * 32
  mov r2, #16                           @ set row counter (16 rows)

.LoopRow:
  ldmia r0!, {r4-r11}                   @ load pixels for current row of frame
  stmia r1!, {r4-r11}                   @ write pixel values to vram
  add r0, r0, r3                        @ move image offset to beginning of frame next row (currently only works for 4 frame animations)
  add r1,  #448                         @ jump to next line VRAM, 480 - 32 to jump to next line at the beginning of the frame row

  subs r2, #1
  bne .LoopRow

  pop {r4-r11}
  bx lr 

.size SpriteFrame, .-SpriteFrame



