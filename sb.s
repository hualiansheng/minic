main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldw	r15, .L11
	@ lr needed for prologue
	ldw	r15, [r15+], #0
	cmpsub.a	r15, #0
	beq	.L2
	ldw	r14, .L11+4
	ldw	r13, [r14+], #0
.L3:
	cmpsub.a	r15, r13
	bsg	.L5
	bsl	.L6
.L5:
	cmpsub.a	r13, #0
	bne	.L6
	cmpsub.a	r15, #0
	beq	.L10
.L6:
	mov	r0, #2
	stw	r0, [r14+], #0
	jump	lr
.L2:
	ldw	r14, .L11+4
	ldw	r13, [r14+], #0
	cmpadd.a	r13, #1
	bne	.L3
	mov	r15, #3
	mov	r0, r15
	stw	r15, [r14+], #0
	jump	lr
.L10:
	mov	r0, #1
	stw	r0, [r14+], #0
	jump	lr
.L12:
	.align	2
.L11:
	.word	a
	.word	b
	.size	main, .-main
	.comm	a, 4, 4
	.comm	b, 4, 4
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
