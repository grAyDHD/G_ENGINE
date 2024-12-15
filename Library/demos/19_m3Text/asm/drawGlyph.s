.arm
.global copyGlyphToVRAM
.type copyGlyphToVRAM, %function
  @ PROCESS:
    @ calculate VRAM offset.  draw glyph here + glyph.yOffset * 240
    @ xOffset used to set pointer to proper image position
    @ fontBitmap incremented by xOffset + (yOffset * 803)

@ copyGlyphToVRAM(int x, int y, GlyphInfo glyphData, const void *fontBitmap);
@ r0 = x
@ r1 = y
@ r2 = glyphData #0 = xOffset, #4 = height, #8 = yOffset
@ r3 = pointer to fontBitmap


copyGlyphToVRAM:
  push {r4-r11}
  @ Step 1: calculate VRAM offset
  mov r4, #240                           @ r4 = screen width
  mul r1, r1, r4                         @ r1 = y * SW
  add r1, r1, r0                         @ r1 = x + (y * SW)
  add r1, r1, r1                         @ r1 doubled for byte offset 
  ldr r0, =0x6000000
  add r0, r0, r1

  @ r0 = VRAM base offset (no glyph yOffset) 
  @ r1 = VRAM byte offset (no longer needed) 
  @ r2 = glyphData struct pointer
  @ r3 = image pointer  (need to increment by xOffset+yOffset*803)
  @ r4 = screen width pxls (may still need)

  @ Step 2: calculate fontBitmap offset
  ldr r5, [r2, #0]                      @ r7 = glyph.xOffset
  ldr r6, [r2, #8]                      @ r8 = glyph.yOffset
  ldr r7, [r2, #4]                      @ r9 = glyph.height
  mul r1, r6, #803                      @ r1 = glyph row offset
  add r1, r1, r5                        @ r1 = total glyph offset
  add r3, r3, r1                        @ r3 = fontBitmap offset

  @ r0 = VRAM base offset (needs glyph yOffset added) 
  @ r1 = glyph offset (no longer need)
  @ r2 = struct (no longer need)
  @ r3 = image pointer glyph offset
  @ r4 = 240 (no longer need)
  @ r5 = glyph.xOffset (no longer need)
  @ r6 = glyph.yOffset (need for VRAM final offset) 
  @ r7 = glyph.height 
  @ r8 = glyph.yOffset (need for final VRAM offset)
   
  @ Step 3: calculate final VRAM offset for drawing glyph
  mul r1, r6, #480                     @ r1 =yOffset x screen width in bytes
  add r0, r0, r1                       @ r0 = final VRAM offset

  @ r0 = VRAM final offset 
  @ r1 = vram yOffset (no longer need)
  @ r2 = struct (no longer need)
  @ r3 = image pointer glyph offset
  @ r4 = 240 (no longer need)
  @ r5 = glyph.xOffset (need for calculating row looping offset)
  @ r6 = glyph.yOffset (no longer need) 
  @ r7 = glyph.height 
  @ r8 = glyph.yOffset  (no longer need)

  @ Step 4: calculate vram row loop and image row loop offsets
  @ I messed up the data structure, I need width AND xOffset





  bx lr

.size copyGlyphToVRAM, .-copyGlyphToVRAM
