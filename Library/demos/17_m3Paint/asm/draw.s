.section .iwram, "ax", %progbits
.align 2
.global drawCircle
.arm
.type drawCircle, %function

fillCircle:
  @ARGUMENTS: Color, center coord., radius


  .size fillCircle, .-fillCircle
