.global printGlyph
.type printGlyph, %function

@ printGlyph(int x, int y, const void *fontBitmap, GlyphInfo *fontDataIndex);

@ r0 = x
@ r1 = y
@ r2 = image base address
@ r3 = GlyphInfo struct address

printGlyph:
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
  @get glyph width from struct into r4
  ldrh r4, [r3, #2]  

  @ VRAM row offset
  mov r5, #480        
  sub r5, r5, r4
  sub r5, r5, r4 

  @ image row offset
  ldr r6, =1268     @Peaberry = 2048, MiniGBA = 1266? 
  sub r6, r6, r4
  sub r6, r6, r4
 

  @ Load transparency value
  ldr r10, =0x7C1F    @ Magenta color (RGB(31,0,31))
  @ Initialize row counter, ultimately set as char height
  mov r7, #11 @Peaberry = 16, MiniGBA = 12

  @ Get xOffset from struct into r9
  ldrh r9, [r3, #0]                                 @ glyph xOffset
  add r2, r2, r9                                    @ image position + xOffset for glyph

.LoopRow:
  mov r8, r4                                        @ initialize width/column counter, resets every row

.LoopPixel:
  ldrh r9, [r2], #2    @ Load 1 pixels, increment image address
  cmp r9, r10
  beq .SkipPixel

  strh r9, [r1], #2    @ Store 1 pixels, increment VRAM address
  subs r8, r8, #1
  bne .LoopPixel
  b .NewRow
  

.SkipPixel: 
  subs r8, r8, #1
  add r1, r1, #2
  bne .LoopPixel      @ Only continue at end of row

.NewRow:
  add r1, r1, r5      @ Add VRAM row offset to VRAM address
  add r2, r2, r6      @ Add image row offset to image address  

  subs r7, r7, #1     @ Decrement row counter
  bne .LoopRow        @ Repeat until all 16 rows are processed


.Done:
  pop {r4-r11}        @ Restore registers
  bx lr               @ Return

                  @ r0 = vram byte offset amount (not needed)
                  @ r1 = VRAM xy address
                  @ r2 = image pointer
                  @ r3 = GlyphInfo
                  @ r4 = character width
                  @ r5 = VRAM row offset
                  @ r6 = image row offset
                  @ r7 = row counter (height)
                  @ r8 = column counter (width)
                  @ r9 = pixel to write
                  @r10 = transparency value

.size printGlyph, .-printGlyph
