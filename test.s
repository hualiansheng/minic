	.text
	.global	main
	.type	main,function
main:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	stw	r3, [r27+], #-23
	ldw	r5, [r27+], #-23
	sub	r29, r29, #27
	add	r3, r27, #-19
	mov	r7, #0
.L3:
	cmpsub.a	r7, #10
	beg	.L4
.L1:
	add	r6, r5, r7
	mov	r4, #122
	sub	r4, r4, r7
	stb	r4, [r6+], #0
	add	r4, r7, #1
	mov	r7, r4
	b	.L3
.L4:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L5:
