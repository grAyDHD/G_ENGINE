.arm
.global SpriteFrame32Bit
.type SpriteFrame32Bit, %function

@ Currently handles 4 frame animations of 32x32 sprite size

@ SpriteFrame32Bit(int x, int y, int frame, const void *image, int frameCount)
@ r0 = x
@ r1 = y
@ r2 = frame
@ r3 = image (pointer to sprite sheet)

SpriteFrame32Bit:
  push {r4-r11}  @8x4 = 32 bytes?
  
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
  mov r4, #64                            @ 32 pixels = 64 bytes per row of a frame
  mul r5, r2, r4                         @ multiply frame number by frame width
  add r0, r3, r5                         @ add frame offset to image base address

                  @ r0 = image frame offset
                  @ r1 = vram xy address
                  @ r2 = frame (no longer needed)
                  @ r3 = image (no longer needed)
                  @ r4 = 64 length of row in bytes, 32px*2

@ Step 3: Loop through each row of the sprite (32 rows)
  ldr r2, [sp, #32]                     @ load 5th argument from stack, 8 registers pushed on stack at 32bits/4bytes each, 4x8=32 
  sub r2, #1
  mul r3, r2, r4                        @ frame count * 64
  mov r2, #32                           @ set row counter (32 rows)

.LoopRow32:
  ldmia r0!, {r4-r11}                   @ load pixels for current row of frame
  stmia r1!, {r4-r11}                   @ write pixel values to vram
  ldmia r0!, {r4-r11}                   @ load pixels for current row of frame
  stmia r1!, {r4-r11}                   @ write pixel values to vram
  add r0, r0, r3                        @ move image offset to beginning of frame next row
  add r1,  #416                         @ jump to next line VRAM, 480 - 64 to jump to next line at the beginning of the frame row

  subs r2, #1
  bne .LoopRow32

  pop {r4-r11}
  bx lr 

.size SpriteFrame32Bit, .-SpriteFrame32Bit
