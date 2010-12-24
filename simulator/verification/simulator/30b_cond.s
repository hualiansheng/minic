	.file	"30b_cond.s"
	.text
	.align	2
	.global	main
	.type	main,function
main:
	@ args = 0, pretend = 0, frame = 12
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
	stw	pc, [sp-], #4;
	stw	lr, [sp-], #8;
	stw	ip, [sp-], #12;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;
	stw	ip, [sp-], #12;stw	fp, [sp-], #16;sub	sp, sp, #16;
	sub	fp, ip, #4
	sub	sp, sp, #12

	mov r0, #0
	mov r1, #1
	mov r2, #2
	cmpsub.a r0, r1
	beq EXIT
	bne L1
	b EXIT
L1:
	buge EXIT
	bult L2
	b EXIT
L2:
	bn L3
	b EXIT
L3:
	bnn EXIT
	bov EXIT
	bnv L4
	b EXIT
L4:
	bugt EXIT
	bule L5
	b EXIT
L5:
	bsge EXIT
	bslt L6
	b EXIT
L6:
	bsgt EXIT
	bsle L7
	b EXIT
L7:
	mov r4, #8
	@ register r4 value 8 is the final result


EXIT:
	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
	.size	main, .-main
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
