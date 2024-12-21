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

.global m3_Background
.type m3_Background, %function
@ m3_Background(const void *src)

m3_Background: @r0 is image src, r1 dest
  push {r3-r12}
  ldr r1, =0x6000000 @vram base address
  mov r2, #1920 @number of total writes to fill screen
.Ldraw:
  ldmia r0!, {r3-r12} @10 words loaded from src image
  stmia r1!, {r3-r12} @writes 10 loaded words to vram
  subs r2, r2, #1
  bhi .Ldraw
  pop {r3-r12}
  bx lr

  .size m3_Background, .-m3_Background



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



