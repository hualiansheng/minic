	.file	"sb.c"
	.text
	.align	2
	.global	main
	.type	main,function
main:
	@ args = 0, pretend = 0, frame = 52
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stw	pc, [sp-], #4;
	stw	lr, [sp-], #8;
	stw	ip, [sp-], #12;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;sub	sp, sp, #16;
	sub	fp, ip, #4
	sub	sp, sp, #52
	ldw	r14, [fp+], #-24
	not	r15, #51
	mov	r14, r14 << #2
	sub	r13, fp, #12
	add	r14, r13, r14
	add	r15, r14, r15
	mov	r14, #1
	stw	r14, [r15+], #0
	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
	.size	main, .-main
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
