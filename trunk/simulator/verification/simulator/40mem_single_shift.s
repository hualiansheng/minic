	.file	"40mem_single_shift.c"
	.text
	.align	2
	.global	main
	.type	main,function
main:
	mov	ip, sp
	stw	pc, [sp-], #4;
	stw	lr, [sp-], #8;
	stw	ip, [sp-], #12;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;sub	sp, sp, #16;
	sub	fp, ip, #4
	sub	sp, sp, #28

	@ left shift 2
	mov r0, #8
	mov r15, #1
	sub r16, fp, #4
	stw r0, [r16-], r15<<#2
	@ address 0x0ffffff4 value 8
	ldw r1, [r16-], r15<<#2
	@ register r1 value 8

	@ right shift 2
	mov r0, #9
	mov r15, #16
	sub r16, fp, #8
	stw r0, [r16-], r15>>#2
	@ address 0x0ffffff0 value 9
	ldw r2, [r16-], r15>>#2
	@ register r2 value 9



	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
	.size	main, .-main
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
