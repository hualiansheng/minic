	.text
	.global	main
	.type	main,function
main:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #112
	add	r3, r27, #-56
	stw	r3, [r27+], #-60
	add	r3, r27, #-104
	stw	r3, [r27+], #-108
	ldw	r8, [r27+], #-60
	ldw	r5, [r27+], #-108
	mov	r9, #0
.L3:
	cmpsub.a	r9, #10
	beg	.L4
	ldw	r7, [r27+], #-64
.L1:
	add	r4, r8, r9<<#2
	ldw	r4, [r4+], #0
	mov	r7, r4
	add	r6, r8, r9<<#2
	add	r4, r5, r9<<#2
	ldw	r4, [r4+], #0
	stw	r4, [r6+], #0
	add	r4, r5, r9<<#2
	stw	r7, [r4+], #0
	add	r4, r9, #1
	mov	r9, r4
	b	.L3
.L4:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L5:
