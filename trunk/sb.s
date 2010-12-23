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
	add	r6, r5, #4
	ldw	r4, [r6+], #0
	add	r4, r4, #1
	mov	r3, #2
	stw	r3, [r6+], #0
	ldw	r4, [r6+], #0
	add	r4, r4, #1
	ldw	r4, [r6+], #0
	add	r4, r4, #1
	mov	r0, r4
.L1:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L2:
