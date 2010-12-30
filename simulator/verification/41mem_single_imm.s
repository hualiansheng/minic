	.file	"41mem_single_imm.c"
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

	sub r0, fp, #2
	mov r1, #8
	stw r1, [r0+], #2
	@ address $fp value 8
	ldw r2, [r0+], #2
	@ register $r2 value 8

	mov r1, #256
	stb r1, [r0+], #2
	@address $fp value 0
	ldb r3, [r0+], #2
	@ register $r3, value 0


	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
	.size	main, .-main
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
