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
  @ Character width
  mov r4, #8          

  @ VRAM row offset
  mov r5, #480        
  sub r5, r5, r4
  sub r5, r5, r4 

  @ image row offset
  ldr r6, =2048       
  sub r6, r6, r4
  sub r6, r6, r4

  
                  @ r0 = vram byte offset amount (not needed)
                  @ r1 = VRAM xy address
                  @ r2 = image pointer
                  @ r3 = empty
                  @ r4 = character width
                  @ r5 = VRAM row offset
                  @ r6 = image row offset

  @ Initialize row counter, ultimately set as char height
  mov r7, #16 

  @ A offset = 0, B=22,C=40 rest in font table

  ldr r3, =1480                @ glyph offset
  add r2, r2, r3              @ character offset for testing

.LoopRow:
  mov r8, r4                  @ initialize width counter

.LoopPixel:
  ldrh r3, [r2], #2    @ Load 1 pixels, increment image address
  strh r3, [r1], #2    @ Store 1 pixels, increment VRAM address
  subs r8, r8, #1
  bne .LoopPixel

  add r1, r1, r5      @ Add VRAM row offset to VRAM address
  add r2, r2, r6      @ Add image row offset to image address  

  subs r7, r7, #1     @ Decrement row counter
  bne .LoopRow        @ Repeat until all 16 rows are processed

  pop {r4-r11}        @ Restore registers
  bx lr               @ Return

.size copyGlyphToVRAM, .-copyGlyphToVRAM
