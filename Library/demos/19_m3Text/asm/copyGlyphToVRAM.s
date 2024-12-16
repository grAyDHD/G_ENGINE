.global copyGlyphToVRAM
.type copyGlyphToVRAM, %function

@ copyGlyphToVRAM(int x, int y, const void *image)
@ r0 = x
@ r1 = y
@ r2 = image (pointer to sprite)

copyGlyphToVRAM:
  push {r4-r11}       @ Save registers on the stack
  
@ Step 1: Calculate VRAM xy offset address
  mov r4, #240        @ Screen width in bytes
  mul r5, r1, r4      @ y * 240
  add r0, r0, r5      @ x + y * 240
  add r0, r0, r0      @ Double for 16-bit color mode
  ldr r1, =0x6000000  @ Base VRAM address
  add r1, r1, r0      @ VRAM address for the top-left corner of the sprite

                  @ r0 = unused
                  @ r1 = VRAM xy address
                  @ r2 = image pointer
  
@ Step 2: Copy each row of the 16x16 sprite into VRAM
  mov r3, #16         @ Row counter (16 rows)
  mov r4, #32         @ Row size in bytes (16 pixels * 2 bytes per pixel)

.LoopRow:
  ldmia r2!, {r5-r11} @ Load 7 words (28 bytes) from the image pointer
  stmia r1!, {r5-r11} @ Store 7 words (28 bytes) into VRAM
  ldr r5, [r2], #4    @ Load the last word (4 bytes) from the row and advance
  str r5, [r1], #4    @ Store the last word into VRAM and advance
  add r1, r1, #448    @ Move to the next line in VRAM (screen width in bytes)
  subs r3, r3, #1     @ Decrement row counter
  bne .LoopRow        @ Repeat until all 16 rows are processed

  pop {r4-r11}        @ Restore registers
  bx lr               @ Return

.size copyGlyphToVRAM, .-copyGlyphToVRAM
