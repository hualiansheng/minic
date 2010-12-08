	.file	"test4.c"
	.global	a
	.data
	.align	2
	.type	a, object
	.size	a, 4
a:
	.word	6
	.text
	.align	2
	.global	Factorial
	.type	Factorial,function
Factorial:
	@ args = 0, pretend = 0, frame = 4
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stw	pc, [sp-], #4;
	stw	lr, [sp-], #8;
	stw	ip, [sp-], #12;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;sub	sp, sp, #16;
	sub	fp, ip, #4
	sub	sp, sp, #4
	stw	r0, [fp+], #-16
	ldw	r15, [fp+], #-16
	cmpsub.a	r15, #1
	bne	.L2
	mov	r15, #1
	b	.L3
.L2:
	ldw	r15, [fp+], #-16
	sub	r15, r15, #1
	mov	r0, r15
	b.l	Factorial
	mov	r15, r0
	ldw	r14, [fp+], #-16
	mul	r15, r14, r15
.L3:
	mov	r0, r15
	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
	.size	Factorial, .-Factorial
	.align	2
	.global	main
	.type	main,function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stw	pc, [sp-], #4;
	stw	lr, [sp-], #8;
	stw	ip, [sp-], #12;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;sub	sp, sp, #16;
	sub	fp, ip, #4
	ldw	r15, .L7
	ldw	r15, [r15+], #0
	mov	r0, r15
	b.l	Factorial
	mov	r14, r0
	ldw	r15, .L7
	stw	r14, [r15+], #0
	mov	r15, #0
	mov	r0, r15
	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
.L8:
	.align	2
.L7:
	.word	a
	.size	main, .-main
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
