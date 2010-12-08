	.file	"test3.c"
	.text
	.align	2
	.global	func
	.type	func,function
func:
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
	add	r14, r15, #1
	ldw	r15, [fp+], #-16
	mul	r15, r14, r15
	stw	r15, [fp+], #-16
	ldw	r15, [fp+], #-16
	mov	r0, r15
	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
	.size	func, .-func
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
	mov	r0, #3
	b.l	func
	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
	.size	main, .-main
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
