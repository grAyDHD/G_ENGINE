.global changeSquareSize
changeSquareSize:
  ldr r1, [r0] @load value square address (size)
  add r1, r1, #1 @increment
  str r1, [r0] @store incremented value at sq.sizeaddress
  bx lr   @return to caller
