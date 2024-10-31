.arm
.global memCopy32
.type memCopy32, %function

@ memCopy32(void *destination, const void *source, u32 wdn);

memCopy32:
	and		r12, r2, #7
	movs	r2, r2, lsr #3
	beq		.Lres_cpy32
	push	{r4-r10}
	@ copy 32byte chunks with 8fold xxmia
.Lmain_cpy32:
		ldmia	r1!, {r3-r10}	
		stmia	r0!, {r3-r10}
		subs	r2, r2, #1
		bhi		.Lmain_cpy32
	pop		{r4-r10}
	@ and the residual 0-7 words
.Lres_cpy32:
		subs	r12, r12, #1
		ldmcsia	r1!, {r3}
		stmcsia	r0!, {r3}
		bhi		.Lres_cpy32
	bx	lr

  .size memCopy32, .-memCopy32

.global m3_Background
.type m3_Background, %function
@ m3_Background(const void *src)

m3_Background: @r0 is image src, r1 dest
  push {r3-r12}
  ldr r1, =0x6000000 @vram base address
  mov r2, #1920 @number of total writes to fill screen
.Ldraw:
  ldmia r0!, {r3-r12} @10 words loaded from src image
  stmia r1!, {r3-r12} @writes 10 loaded words to vram
  subs r2, r2, #1
  bhi .Ldraw
  pop {r3-r12}
  bx lr

  .size m3_Background, .-m3_Background
