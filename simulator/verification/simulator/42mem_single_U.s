	.file	"42mem_single_U.s"
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
	sub	sp, sp, #12

	mov r0, #8
	stw r0, [fp-], #2
	@ address $fp-2 value 8
	ldw r1, [fp-], #2
	@ register r1 value 8
	mov r0, #9
	stb r0, [sp+], #2
	@ address $sp+2 value 8
	ldb r0, [sp+], #2
	@ register r0 value 8


	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
	.size	main, .-main
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
