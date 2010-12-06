	.file	"test.c"
	.comm	a, 4, 4
	.text
	.align	2
	.global	Factorial
	.type	Factorial,function
Factorial:
	@ args = 0, pretend = 0, frame = 4
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stm.w	(r23, r24, sl, fp, ip, lr, pc), [sp-]
	sub	fp, ip, #4
	sub	sp, sp, #4
	stw	r0, [fp+], #-28
	ldw	r15, [fp+], #-28
	cmpsub.a	r15, #1
	bne	.L2
	mov	sl, #1
	b	.L3
.L2:
	ldw	r14, [fp+], #-28
	sub	r15, r14, #1
	mov	r0, r15
	b.l	Factorial
	mov	r24, r0
	ldw	r23, [fp+], #-28
	mul	sl, r23, r24
.L3:
	mov	r15, sl
	mov	r0, r15
	ldm	(r23, r24, sl, fp, sp, pc), [fp-]
	.size	Factorial, .-Factorial
	.align	2
	.global	main
	.type	main,function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stm.w	(r24, sl, fp, ip, lr, pc), [sp-]
	sub	fp, ip, #4
	ldw	r15, .a
	mov	r14, #1
	stw	r14, [r15+], #0
	mov	r0, #6
	b.l	Factorial
	mov	r24, #0
	mov	sl, r24
	mov	r0, sl
	ldm	(r24, sl, fp, sp, pc), [fp-]
.L8:
	.align	2
.a:
	.word	a
	.size	main, .-main
	.ident	"GCC: (UC4_1.0-beta_20100415) 4.4.2"
