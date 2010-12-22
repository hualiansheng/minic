	.comm	i, 4, 4
	.comm	a, 40000000, 4
	.comm	q, 4, 4
	.comm	c, 1, 1
	.comm	p, 1, 4
	.comm	s, 98, 4
	.section	.rodata
.LC0:
	.ascii	"hualiansheng\000"
.LC1:
	.ascii	"abcdefg\000"
	.text
	.global	f
	.type	f,function
f:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	stw	r0, [r27+], #-16
	ldw	r5, .L2+0
	sub	r29, r29, #24
	ldw	r1, .L2+4
	add	r5, r5, #1
	ldw	r4, [r1+], #0
	stb	r4, [r5+], #0
	ldw	r6, .L2+8
	mov	r0, r6
	ldw	r6, .L2+12
.L1:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L2:
	.word	s
	.word	c
	.word	.LC0
	.word	.LC1
	.section	.rodata
.LC2:
	.ascii	"\000"
.LC3:
	.ascii	"penglaobandashabi\000"
.LC4:
	.ascii	"\n\000"
	.text
	.global	main
	.type	main,function
main:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #20
	ldw	r10, .L4+0
	ldw	r1, .L4+4
	ldw	r8, [r1+], #0
	ldw	r7, [r27+], #-16
	ldw	r1, .L4+8
	ldw	r6, [r1+], #0
	ldw	r1, .L4+12
	ldw	r5, [r1+], #0
	add	r4, r10, #4
	mov	r3, #1
	stw	r3, [r4+], #0
	add	r4, r10, #8
	mov	r3, #1
	stw	r3, [r4+], #0
	add	r9, r10, #12
	add	r4, r10, #8
	ldw	r4, [r4+], #0
	stw	r4, [r9+], #0
	add	r4, r8, #500
	mov	r7, r4
	ldw	r4, .L4+4
	mov	r6, r4
	ldw	r1, .L4+8
	stw	r6, [r1+], #0
	ldw	r4, .L4+16
	mov	r5, r4
	ldw	r1, .L4+12
	stw	r5, [r1+], #0
	ldw	r4, .L4+20
	mov	r5, r4
	ldw	r1, .L4+12
	stw	r5, [r1+], #0
	ldw	r4, .L4+24
	mov	r5, r4
	ldw	r1, .L4+12
	stw	r5, [r1+], #0
	stw	r7, [r27+], #-16
	mov	r0, r5
	b.l	print_string
	ldw	r10, .L4+0
	ldw	r1, .L4+4
	ldw	r8, [r1+], #0
	ldw	r7, [r27+], #-16
	ldw	r1, .L4+8
	ldw	r6, [r1+], #0
	ldw	r1, .L4+12
	ldw	r5, [r1+], #0
	ldw	r4, .L4+28
	stw	r7, [r27+], #-16
	mov	r0, r4
	b.l	print_string
	ldw	r10, .L4+0
	ldw	r1, .L4+4
	ldw	r8, [r1+], #0
	ldw	r7, [r27+], #-16
	ldw	r1, .L4+8
	ldw	r6, [r1+], #0
	ldw	r1, .L4+12
	ldw	r5, [r1+], #0
	add	r4, r10, #4
	ldw	r4, [r4+], #0
	mov	r0, r4
	b.l	f
	mov	r4, r0
	mov	r5, r4
	ldw	r1, .L4+12
	stw	r5, [r1+], #0
	mov	r0, r7
.L3:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L4:
	.word	a
	.word	i
	.word	q
	.word	p
	.word	.LC2
	.word	.LC0
	.word	.LC3
	.word	.LC4
