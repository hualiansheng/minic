	.comm	i, 4, 4
	.comm	a, 40000000, 4
	.comm	q, 4, 4
	.comm	c, 1, 1
	.comm	p, 1, 4
	.comm	s, 98, 4
	.text
	.global	f
	.type	f,function
f:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #24
	stw	r0, [r27+], #-16
	ldw	r4, .L2+0
	mov	r0, r4
.L1:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L2:
	.word	s
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
	ldw	r1, .L4+0
	ldw	r9, [r1+], #0
	ldw	r5, .L4+8
	ldw	r1, .L4+12
	ldw	r4, [r1+], #0
	add	r6, r9, #500
	mov	r8, r6
	ldw	r6, .L4+0
	mov	r7, r6
	ldw	r1, .L4+4
	stw	r7, [r1+], #0
	add	r6, r5, #4
	ldw	r4, [r6+], #0
	mov	r0, r4
	b.l	f
	mov	r4, r0
	mov	r0, r8
.L3:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L4:
	.word	i
	.word	q
	.word	a
	.word	p
