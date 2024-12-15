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
  mul r5, r1, r4                         @ r5 = y * 240
  add r5, r5, r0                         @ r5 = x + (y * 240)
  add r5, r5, r5                         @ r5 doubled for byte offset 
  ldr r6, =0x6000000
  add r6, r6, r5

  @ r0 = x (don't need anymore)
  @ r1 = y (don't need anymore)
  @ r2 = glyphData struct pointer
  @ r3 = image pointer  (need to increment by yOffset*803)
  @ r4 = screen width pxls 
  @ r5 = VRAM byte offset no longer needed 
  @ r6 = VRAM base offset (still need to add yOffset*480 from glyph data)

  @ Step 2: calculate fontBitmap offset
  ldr r7, [r2, #0]                      @ r7 = glyph.xOffset
  ldr r8, [r2, #8]                      @ r8 = glyph.yOffset
  ldr r9, [r2, #4]                      @ r9 = glyph.height
  mul r5, r8, #803                      @ r5 = glyph row offset
  add r5, r5, r7                        @ r5 = total glyph offset
  add r3, r3, r5                        @ r3 = fontBitmap offset

  @ r0 = x (don't need)
  @ r1 = y (don't need)
  @ r2 = struct (still need to load [r2, #4]) for glyph height
  @ r3 = image pointer glyph offset
  @ r4 = 240
  @ r5 = glyph offset (don't need)
  @ r6 = VRAM base offset 
  @ r7 = glyph.xOffset (don't need)
  @ r8 = glyph.yOffset (need for final VRAM offset)

  @ Step 3: add (yOffset * 240) * 2 to VRAM base offset for final glyph offset
  mul r8, r8, r4                        @ r8 = yOffset * 240
  add r8, r8, r8                        @ r8 = yOffset in bytes for VRAM 
  add r6, r6, r8                        @ r6 = final VRAM offset

  @ r0 = x (don't need)
  @ r1 = y (don't need)
  @ r2 = struct (don't need) 
  @ r3 = image pointer glyph offset
  @ r4 = 240 (don't need)
  @ r5 = glyph offset (don't need)
  @ r6 = VRAM base offset 
  @ r7 = glyph.xOffset (don't need)
  @ r8 = vram y byte offset (don't need)
  @ r9 = glyph height pxls

  @ Step 4:






  bx lr

.size copyGlyphToVRAM, .-copyGlyphToVRAM
