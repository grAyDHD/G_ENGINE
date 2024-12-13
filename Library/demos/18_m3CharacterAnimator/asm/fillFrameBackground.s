.arm
.global fillFrameBackground
.type fillFrameBackground, %function

  @ Steps: first, take x/y coordinates of parameters, calculate proper vram starting offset
  @ then loop row by row, any pixels of color value used for transparency layer-
  @ are set to mathing pixel from background image data
  @ but no vram offset used in that part of calculation

@ fillFrameBackground(int x, int y, int size, const void *IMAGE)
@ r0 = x
@ r1 = y
@ r2 = size
@ r3 = image (pointer to sprite sheet)

fillFrameBackground:
  push {r4-r11}  @ 8 registers, 32 bits or 4 bytes each = 32 bytes onto stack

  @ Step 1: calculate VRAM  and IMAGE offsets
  mov r4, #240                           @ r4 = y base offset value
  mul r5, r1, r4                         @ r5 = y * 240
  add r5, r5, r0                         @ r5 = x + (y * 240)
  add r5, r5, r5                         @ r5 doubled for byte offset 

  ldr r1, =0x6000000                     @ r1 = start of VRAM
  add r0, r3, r5                         @ r0 = image offset
  add r1, r1, r5                         @ r1 = vram offset 
  ldr r3, =0x7C1F                        @ r3 = transparency value

  mov r4, #480                           @ screen width in bytes
  sub r4, r4, r2
  sub r4, r4, r2                         @ subtract size times 2 by subbing twice

  @ Outer loop over rows
  mov r5, r2                             @ r8 = rowCounter = size
RowLoop:
  mov r6, r2                             @ r9 = colCounter = size
                  @ r0 = image offset
                  @ r1 = vram offset
                  @ r2 = size in pixels
                  @ r3 = transparency value
                  @ r4 = next row offset from last pixel current row
                  @ r5 = row counter
                  @ r6 = column counter

  

ColLoop:
  ldrh r7, [r1]                         @ r10 = pixel from Vram
  cmp r3, r7                            @ Compare pxl from VRAM to transparency color
  @ r0=image offset, r1=vram offset, r2=size, r3=transparency, r4=next row offset, r5=rowC
  @ r6=colC @ r7=vramSpritePxl

  bne SkipRestore                        @ If equal, no restore

  ldrh r7, [r0]                         @ Load background pixel
  strh r7, [r1]                         @ Write bg pixel to VRAM

SkipRestore:
  add r0, r0, #2                         @ Increment IMAGE offset
  add r1, r1, #2                         @ Increment VRAM offset 
  subs r6, r6, #1                        @ Decrement column counter
  bne ColLoop                            @ if not done with current row, continue ColLoop

  @ End of row: move to start of next row
  add r1, r1, r4                        @ VRAM pointer to start of of next row
  add r0, r0, r4                        @ IMAGE pointer to start of next row

  subs r5, r5, #1
  bne RowLoop
  pop {r4-r11}
  bx lr

.size fillFrameBackground, .-fillFrameBackground
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.global clearSpriteFrame
.type clearSpriteFrame, %function
  
@ clearSpriteFrame(int x, int y, int size, const void *IMAGE)
@ r0 = x
@ r1 = y
@ r2 = size
@ r3 = image (pointer to sprite sheet)

clearSpriteFrame:
  push {r4-r11}

  @ Step 1: calculate VRAM and IMAGE byte offsets
  mov r4, #240
  mul r5, r1, r4                         @ r5 = y * 240
  add r5, r5, r0                         @ r5 = x + (y * 240)
  add r5, r5, r5                         @ r5 doubled for byte offset 


  ldr r1, =0x6000000                     @ r1 = start of VRAM
  add r0, r3, r5                         @ r0 = image offset
  add r1, r1, r5                         @ r1 = vram offset 

  mov r3, #480                           @ screen width in bytes
  sub r3, r3, r2
  sub r3, r3, r2                         @ subtract size times 2 by subbing twice

  lsr r4, r2, #3                         @shift size right 3, or divide by 8
  
@ row counter = size, as height = size
                  @ r0 = image offset
                  @ r1 = vram offset
                  @ r2 = size in pixels, row counter
                  @ r3 = next row offset
                  @ r4 = number of 8-pixel chunks per row
                  @ r5 = chunk counter
RowsLoop:
  mov r5, r4
  
ChunkLoop:
  ldmia r0!, {r6-r9} @10 words loaded from src image
  stmia r1!, {r6-r9} @writes 10 loaded words to vram
  subs r5, r5, #1
  bne ChunkLoop

  @ Move to next row
  add r0, r0, r3
  add r1, r1, r3

  subs r2, r2, #1
  bne RowsLoop

  pop {r4-r11}
  bx lr

.size clearSpriteFrame, .-clearSpriteFrame
