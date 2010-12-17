	.file	"test.c"
	.text
	.align	2
	.global	f
	.type	f,function
f:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stw	lr, [sp-], #4;
	stw	sl, [sp-], #8;
	stw	r24, [sp-], #12;
	stw	r24, [sp-], #12;stw	r23, [sp-], #16;
	stw	r24, [sp-], #12;stw	r23, [sp-], #16;sub	sp, sp, #16;
	sub	r24, r1, #1
	sub.a	sl, r0, #0
	mov	r23, #0
	beq	.L3
.L6:
	sub	r0, sl, #1
	mov	r1, r24
	b.l	f
	sub.a	sl, sl, #2
	add	r23, r23, r0
	sub	r24, r24, #2
	bne	.L6
.L3:
	mov	r0, r23
	ldw.w	r23,  [sp]+, #4
	ldw.w	r24,  [sp]+, #4
	ldw.w	sl,  [sp]+, #4
	ldw.w	ip,  [sp]+, #4
	jump	ip
	.size	f, .-f
	.align	2
	.global	main
	.type	main,function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r0, #1
	mov	r1, #2
	@ lr needed for prologue
	@ Sibcall epilogue
	b	f
	.size	main, .-main
	.global	k
	.data
	.align	2
	.type	k, object
	.size	k, 4
k:
	.word	1
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
