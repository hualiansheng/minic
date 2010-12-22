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
	sub	r29, r29, #24
	stw	r0, [r27+], #-16
	ldw	r5, .L2+0
	ldw	r1, .L2+4
	ldw	r4, [r1+], #0
	ldw	r6, .L2+8
	ldw	r6, .L2+12
	add	r5, r5, #1
	stb	r4, [r5+], #0
	mov	r0, r6
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
