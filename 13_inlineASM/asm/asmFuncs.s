	.file	"asmFuncs.s"

	.text
	.align	2
	.global	increaseSquareSize
  .global decreaseSquareSize
	.code	16
increaseSquareSize:
  ldr r1, [r0] @load value at square address (size)
  add r1, r1, #1 @increment
  str r1, [r0] @store incremented value at sq.sizeaddress
  bx lr   @return to caller
decreaseSquareSize:
  ldr r1, [r0] @r0 is square pointer,[r0] to load value at address
  sub r1, r1, #1
  str r1, [r0]
  bx lr
