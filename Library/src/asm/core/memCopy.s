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
