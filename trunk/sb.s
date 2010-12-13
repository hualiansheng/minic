	.file	"sb.c"
	.text
	.align	2
	.global	f
	.type	f,function
f:
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
	mov	r0, r15
	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
	.size	f, .-f
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
	ldw	r15, [fp+], #-24
	cmpsub.a	r15, #0
	beq	.L4
	ldw	r15, [fp+], #-20
	cmpsub.a	r15, #0
	beq	.L4
	ldw	r15, [fp+], #-24
	cmpsub.a	r15, #0
	beq	.L4
	ldw	r15, [fp+], #-20
	cmpsub.a	r15, #0
	bne	.L5
	ldw	r15, [fp+], #-24
	cmpsub.a	r15, #0
	beq	.L4
.L5:
	ldw	r15, [fp+], #-24
	cmpsub.a	r15, #0
	beq	.L6
	ldw	r15, [fp+], #-20
	cmpsub.a	r15, #0
	beq	.L6
	ldw	r0, [fp+], #-16
	b.l	f
	mov	r0, r0	@ nop
	b	.L8
.L6:
	ldw	r15, [fp+], #-16
	add	r15, r15, #1
	mov	r0, r15
	b.l	f
	b	.L8
.L4:
	ldw	r15, [fp+], #-16
	add	r15, r15, #2
	mov	r0, r15
	b.l	f
.L8:
	ldw	r15, [fp+], #-16
	mov	r0, r15
	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
	.size	main, .-main
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
