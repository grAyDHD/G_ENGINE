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
@ r2 = glyphData struct pointer #0 = xOffset, #4 = yOffset, #8 = width, #12 = height
@ r3 = fontBitmap pointer


copyGlyphToVRAM:
  push {r4-r11}
  @ Step 1: calculate base VRAM offset
  ldr r4, =240
  mul r1, r4, r1                       @ r1 = y * SW
  add r1, r1, r0                         @ r1 = x + (y * SW)
  add r1, r1, r1                         @ r1 doubled for byte offset 
  ldr r0, =0x6000000
  add r0, r0, r1

  @ r0 = VRAM base offset (no glyph yOffset) 
  @ r2 = glyphData struct pointer
  @ r3 = image pointer  (need to increment by xOffset+yOffset*803)

  @ Step 2: calculate fontBitmap offset
  ldr r5, [r2, #0]                      @ r5 = glyph.xOffset
  ldr r6, [r2, #4]                      @ r6 = glyph.yOffset
  ldr r7, [r2, #8]                      @ r7 = glyph.width
  ldr r8, [r2, #12]                     @ r8 = glyph.height

  ldr r4, =803
  mul r1, r6, r4                      @ r1 = glyph row offset
  add r1, r1, r5                        @ r1 = total glyph offset
  add r3, r3, r1                        @ r3 = fontBitmap offset

  @ r0 = VRAM base offset (needs glyph yOffset added) 
  @ r3 = image + glyph offset
  @ r6 = glyph.yOffset (need for VRAM final offset) 
  @ r7 = glyph.width 
  @ r8 = glyph.height
   
  @ Step 3: calculate final VRAM offset for drawing glyph
  ldr r4, =480
  mul r1, r6, r4                     @ r1 =yOffset x screen width in bytes, if no offset is 0
  add r0, r0, r1                       @ r0 = final VRAM offset

  @ r0 = VRAM glyph offset 
  @ r3 = fontBitmap glyph offset
  @ r5 = glyph.xOffset 
  @ r6 = glyph.yOffset
  @ r7 = glyph.width
  @ r8 = glyph.height

  @ Step 4: calculate vram row loop and image row loop offsets
  @ VRAM row loop offset = 480 - (glyph.width * 2)
  @ image row loop offset = 803 - (glyph.width)

  lsl r1, r7, #2                       @ r1 = glyph.width * 2
  ldr r4, =480
  sub r1, r4, r1                     @ r1 = VRAM row loop offset

  ldr r4, =803
  sub r2, r4, r7                     @ r2 = fontBitmap row loop offset

  @ r0 = VRAM glyph offset 
  @ r1 = VRAM row loop offset
  @ r2 = fontBitmap row loop offset
  @ r3 = fontBitmap glyph offset

  @ r7 = glyph.width
  @ r8 = glyph.height

  @ Step 5: set up row/col loops based on glyph.width and glyph.height
  @ copying current fontImage position to VRAM and incrementing both
  mov r4, r8                            @ r4 = row counter

rowLoop:
  cmp r4, #0
  beq return

  mov r5, r7                            @ r5 = column counter

columnLoop:
  cmp r5, #0
  beq endRow
  
  ldrh r6, [r3], #2                     @ r6 = pixel from fontBitmap 
  strh r6, [r0], #2                     @ store pixel in vram

  sub r5, r5, #1                        @ decrement column counter
  b columnLoop

endRow: @ add vram and image row offsets
  add r0, r0, r1                        @ r0 = vram offset + row offset
  add r3, r3, r2                        @ r3 = fontBitmap offset + image row offset

  sub r4, r4, #1                        @ decrement row counter
  b rowLoop

return:
  pop {r4-r11}
  bx lr

.size copyGlyphToVRAM, .-copyGlyphToVRAM
