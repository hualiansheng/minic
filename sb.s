	.file	"sb.c"
	.comm	i, 4, 4
	.comm	a, 40000000, 4
	.comm	q, 4, 4
	.comm	c, 1, 1
	.comm	p, 4, 4
	.comm	s, 98, 4
	.text
	.align	2
	.global	f
	.type	f,function
f:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stw	pc, [sp-], #4;
	stw	lr, [sp-], #8;
	stw	ip, [sp-], #12;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;sub	sp, sp, #16;
	sub	fp, ip, #4
	sub	sp, sp, #8
	stw	r0, [fp+], #-20
	ldw	r15, .L3
	stw	r15, [fp+], #-16
	ldw	r15, .L3+4
	ldb	r14, [r15+], #0	@ zero_extendqisi2
	ldw	r15, .L3
	stb	r14, [r15+], #1
	ldw	r15, [fp+], #-16
	mov	r0, r15
	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
.L4:
	.align	2
.L3:
	.word	s
	.word	c
	.size	f, .-f
	.align	2
	.global	main
	.type	main,function
main:
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
	ldw	r15, .L7
	mov	r14, #1
	stw	r14, [r15+], #4
	ldw	r15, .L7
	mov	r14, #1
	stw	r14, [r15+], #8
	ldw	r15, .L7
	ldw	r14, [r15+], #8
	ldw	r15, .L7
	stw	r14, [r15+], #12
	ldw	r15, .L7+4
	ldw	r15, [r15+], #0
	add	r15, r15, #500
	stw	r15, [fp+], #-16
	ldw	r15, .L7+8
	ldw	r14, .L7+4
	stw	r14, [r15+], #0
	ldw	r15, .L7
	ldw	r15, [r15+], #4
	mov	r0, r15
	b.l	f
	mov	r14, r0
	ldw	r15, .L7+12
	stw	r14, [r15+], #0
	ldw	r15, [fp+], #-16
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
	.word	i
	.word	q
	.word	p
	.size	main, .-main
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
