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


  @ Initialize constants for character width and row offsets
  mov r4, #8          @ Character width
  mov r5, #480        @ VRAM row offset
  sub r5, r5, r4
  sub r5, r5, r4
  mov r6, #64         @ image row offset
  sub r6, r6, r4
  sub r6, r6, r4

  @ Initialize row counter
  mov r7, #16
  
                  @ r0 = vram byte offset amount (not needed)
                  @ r1 = VRAM xy address
                  @ r2 = image pointer
                  @ r3 = empty
                  @ r4 = character width
                  @ r5 = VRAM row offset
                  @ r6 = image row offset
  add r2, r2, #0
  @A offset = 0, B offset = 12, C offset =

.LoopRow:
  ldr r3, [r2], #4    @ Load 2 pixels, increment image address
  str r3, [r1], #4    @ Store 2 pixels, increment VRAM address
  ldr r3, [r2], #4    @ Load 2 pixels, increment image address
  str r3, [r1], #4    @ Store 2 pixels, increment VRAM address
  ldr r3, [r2], #4    @ Load 2 pixels, increment image address
  str r3, [r1], #4    @ Store 2 pixels, increment VRAM address
  ldr r3, [r2], #4    @ Load 2 pixels, increment image address
  str r3, [r1], #4    @ Store 2 pixels, increment VRAM address

  add r1, r1, r5      @ Add VRAM row offset to VRAM address
  add r2, r2, r6      @ Add image row offset to image address  

  subs r7, r7, #1     @ Decrement row counter
  bne .LoopRow        @ Repeat until all 16 rows are processed

  pop {r4-r11}        @ Restore registers
  bx lr               @ Return

.size copyGlyphToVRAM, .-copyGlyphToVRAM
