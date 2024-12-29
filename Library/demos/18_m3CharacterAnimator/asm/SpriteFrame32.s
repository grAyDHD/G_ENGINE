.arm
.global SpriteFrame32Bit
.type SpriteFrame32Bit, %function

@ 4 state, 4 direction, 4 frame animations

@ SpriteFrame32Bit(*image);
@ r0 = PositionComponent struct {int x, int y}
@ r1 = AnimationComponent struct {frame, direction, state}
@ r2 = image (pointer to sprite sheet)
  

SpriteFrame32Bit:
  push {r4-r11}  @8x4 = 32 bytes?

  ldr r0, =0x6000000

  ldmia r2!, {r4-r11}                   @ load pixels from image
  stmia r0!, {r4-r11}                   @ write pixels vram

  pop {r4-r11}
  bx lr 

.size SpriteFrame32Bit, .-SpriteFrame32Bit
