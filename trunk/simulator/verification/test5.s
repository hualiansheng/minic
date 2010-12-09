	.file	"test5.c"
	.text
	.align	2
	.global	main
	.type	main,function
main:
	@ args = 0, pretend = 0, frame = 12
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stm.w	(fp, ip, lr, pc), [sp-]
	sub	fp, ip, #4
	sub	sp, sp, #12

	mov	r1, #1
	cmpsub.a	r1, #1
	cmoveq	r2, #1

	mov	r13, #0
	stw	r13, [fp+], #-16
	ldw	r14, [fp+], #-24
	ldw	r15, [fp+], #-20
	cmpsub.a	r14, r15
	beg	.L2
	mov	r15, #1
	stw	r15, [fp+], #-16
.L2:
	ldw	r13, [r29+], #-16
	stw	r13, [fp+], #-24
	mov	r14, #0
	cmoveq	r15, #1
	mov	r15, r14
	mov	r0, r15
	ldm	(fp, sp, pc), [fp-]
	.size	main, .-main
	.ident	"GCC: (UC4_1.0-beta_20100415) 4.4.2"
