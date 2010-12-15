	.global	main
	.type main function
main:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #72
	add	r3, r27, #-56
	stw	r3, [r27+], #-60
	mov	r5, #0
.L3:
	cmpsub.a	r5, #10
	beg	.L2
.L1:
	rsub	r6, r5, #10
	ldw	r4, [r27+], #-60
	stw	r6, [r4+], r5<<#2
	add	r5, r5, #1
	b	.L3
.L2:
	mov	r5, #0
.L11:
	cmpsub.a	r5, #10
	beg	.L5
.L4:
	add	r6, r5, #1
	mov	r9, r6
.L10:
	cmpsub.a	r9, #10
	beg	.L7
.L6:
	ldw	r3, [r4+], r9<<#2
	mov	r7, r3
	ldw	r3, [r4+], r5<<#2
	mov	r6, r3
	cmpsub.a	r7, r6
	beg	.L9
.L8:
	ldw	r3, [r4+], r5<<#2
	mov	r6, r3
	mov	r8, r6
	ldw	r3, [r4+], r9<<#2
	mov	r6, r3
	stw	r6, [r4+], r5<<#2
	stw	r8, [r4+], r9<<#2
.L9:
	add	r6, r9, #1
	mov	r9, r6
	b	.L10
.L7:
	add	r5, r5, #1
	b	.L11
.L5:
	mov	r0, #0
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
