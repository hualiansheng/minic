	.file	"test.c"
	.global	c
	.data
	.type	c, object
	.size	c, 1
c:
	.byte	97
	.global	a
	.bss
	.align	2
	.type	a, object
	.size	a, 4
a:
	.space	4
	.global	s
	.data
	.align	3
	.type	s, object
	.size	s, 100
s:
	.ascii	"abc\000"
	.space	96
	.section	.rodata
	.align	3
.LC0:
	.ascii	"int : %d ; char : %c ; string : %s %d %d %d %d\012\000"
	.text
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
	sub	sp, sp, #16
	ldw	r13, .L3
	ldw	r15, .L3+4
	ldw	r14, [r15+], #0
	ldw	r15, .L3+8
	ldb	r15, [r15+], #0	@ zero_extendqisi2
	mov	r15, r15 << #24
	mov	r15, r15 |> #24
	ldw	r12, .L3+4
	ldw	r9, [r12+], #0
	ldw	r12, .L3+4
	ldw	r10, [r12+], #0
	ldw	r12, .L3+4
	ldw	r11, [r12+], #0
	ldw	r12, .L3+4
	ldw	r12, [r12+], #0
	stw	r9, [sp+], #0
	stw	r10, [sp+], #4
	stw	r11, [sp+], #8
	stw	r12, [sp+], #12
	mov	r0, r13
	mov	r1, r14
	mov	r2, r15
	ldw	r3, .L3+12
	b.l	printf
	mov	r15, #0
	mov	r0, r15
	mov	ip, fp
	ldw	fp,  [fp-], #12
	ldw	sp,  [ip-], #8
	ldw	ip,  [ip-], #4
	jump	ip
.L4:
	.align	2
.L3:
	.word	.LC0
	.word	a
	.word	c
	.word	s
	.size	main, .-main
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
