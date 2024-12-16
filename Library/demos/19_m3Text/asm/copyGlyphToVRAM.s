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

                  @ r1 = VRAM xy address
                  @ r2 = image pointer
  @ Initialize constants for character width and row offsets
  mov r4, #10         @ Character width
  mov r5, #460        @ VRAM row offset
  mov r6, #22         @ image row offset

  @ Initialize row counter
  mov r7, #1
  

.LoopRow:
  ldr r2, [r2], #4    @ Load 2 pixels, increment image pointer
  str r1, [r1], #4    @ Store 2 pixels, increment VRAM pointer
  ldr r2, [r2], #4    @ Load 2 pixels, increment image pointer
  str r1, [r1], #4    @ Store 2 pixels, increment VRAM pointer
  ldr r2, [r2], #4    @ Load 2 pixels, increment image pointer
  str r1, [r1], #4    @ Store 2 pixels, increment VRAM pointer
  ldr r2, [r2], #4    @ Load 2 pixels, increment image pointer
  str r1, [r1], #4    @ Store 2 pixels, increment VRAM pointer
  ldr r2, [r2], #4    @ Load 2 pixels, increment image pointer
  str r1, [r1], #4    @ Store 2 pixels, increment VRAM pointer

@  add r2, r2, r5      @ Add VRAM row offset to VRAM pointer
@  add r1, r1, r6      @ Add image row offset to image pointer
   

  subs r3, r3, #1     @ Decrement row counter
  bne .LoopRow        @ Repeat until all 16 rows are processed

  pop {r4-r11}        @ Restore registers
  bx lr               @ Return

.size copyGlyphToVRAM, .-copyGlyphToVRAM
