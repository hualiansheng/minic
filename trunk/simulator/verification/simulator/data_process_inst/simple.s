	.file	"simple.c"
	.section	.rodata
	.align	3
.LINDEX:
	.word .LC0
.LC0:
	.ascii	"fuck!!\000"
	.text
	.align	2
	.global	main
	.type	main,function
main:
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
	ldw	r15, .L3
	stw	r15, [fp+], #-20
	ldw	r15, .L3+4
	stw	r15, [fp+], #-16
	ldw	r15, [fp+], #-20
	mov	r0, r15
	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
.L4:
	.align	2
.L3:
	.word 	.LINDEX
	.word	1717982549
	.word	.LC0
	.size	main, .-main
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
