.arm
.global SpriteFrame
.type SpriteFrame, %function
@ SpriteFrame(int x, int y, int frame, const void *image)

@ r0 = x
@ r1 = y
@ r2 = frame
@ r3 = image (pointer to sprite sheet)

SpriteFrame:
  push {r4-r11}
  
  @ Step 1: calculate VRAM base address
  ldr r8, =0x6000000
  @ldr r9, =240 @y base offset value
  @mul r10, r1, r9 @y*offset in r10
  @add r8, r0, r10, lsl #1 @add x+y offset and double result for 2 byte pixels

  @VRAM offset in r8
   
  @ Step 2: Calculate frame offset in image
  mov r4, #32  @16 pixels = 32 bytes per row
  @frame 0, = 0.  frame 1 = 32, frame 2 =  64, etc.
  mul r5, r2, r4 @ multiply frame by pixels in bytes, frame 0 = no offset, start of image.
  add r3, r3, r5 @ jump to frame offset, r3 = image +frame offset

  @ Step 3: Loop through each row of the sprite (16 rows)
  mov r6, #16 @set row counter (16 rows)
.LoopRow:
  ldmia r3!, {r4-r7}
  stmia r8!, {r4-r7}

  ldmia r3!, {r4-r7}
  stmia r8!, {r4-r7}  @ draws one full row by this line

  add r3, #96 @move image offset to beginning of frame next row
 add r8, r8, #448 @jump to next line VRAM, but subtract 32 for 16 pixels or beginning of frame

  subs r6, r6, #1 @ decrement row counter
  bhi .LoopRow

  pop {r4-r11}
  bx lr 

.size SpriteFrame, .-SpriteFrame



