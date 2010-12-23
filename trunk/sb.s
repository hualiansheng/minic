	.comm	a, 4, 4
	.text
	.global	main
	.type	main,function
main:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #64
	add	r3, r27, #-52
	stw	r3, [r27+], #-56
	ldw	r5, [r27+], #-56
	ldw	r4, [r27+], #-60
	mov	r3, #1
	ldw	r1, .L2+0
	stw	r3, [r1+], #0
	mov	r3, #2
	ldw	r1, .L2+0
	stw	r3, [r1+], #0
	add	r5, r5, #4
	mov	r3, #4
	stw	r3, [r5+], #0
	mov	r3, #3
	stw	r3, [r5+], #0
	mov	r0, r4
.L1:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L2:
	.word	a
