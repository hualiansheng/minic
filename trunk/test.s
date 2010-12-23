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
	sub	r29, r29, #28
	mov	r7, #0
	mov	r6, #0
	cmpsub.a	r7, #0
	beq	.L2
.L1:
	add	r5, r7, r6
	add	r4, r7, r6
	add	r4, r5, r4
	b	.L3
.L2:
	add	r4, r7, r6
.L3:
	add	r4, r7, r6
	mov	r0, r4
.L4:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L5:
