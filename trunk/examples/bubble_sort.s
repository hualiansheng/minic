	.global	main
	.type main, function
main:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #544
	add	r3, r27, #-528
	stw	r3, [r27+], #-532
	ldw	r4, [r27+], #-532
	mov	r5, #0
.L3:
	cmpsub.a	r5, #128
	beg	.L2
.L1:
	stw	r4, [r29+], #-4
	stw	r5, [r29+], #-8
	sub	r29, r29, #8
	mov	r0, #128
	b.l	rand_int
	add	r29, r29, #8
	ldw	r4, [r29+], #-4
	ldw	r5, [r29+], #-8
	mov	r6, r0
	stw	r6, [r4+], r5<<#2
	add	r5, r5, #1
	b	.L3
.L2:
	mov	r5, #0
.L6:
	cmpsub.a	r5, #128
	beg	.L5
	ldw	r9, [r27+], #-536
	ldw	r8, [r27+], #-540
.L4:
	ldw	r3, [r4+], r5<<#2
	mov	r6, r3
	stw	r4, [r29+], #-4
	stw	r5, [r29+], #-8
	sub	r29, r29, #8
	mov	r0, r6
	b.l	print_int
	add	r29, r29, #8
	ldw	r4, [r29+], #-4
	ldw	r5, [r29+], #-8
	stw	r4, [r29+], #-4
	stw	r5, [r29+], #-8
	sub	r29, r29, #8
	mov	r0, #10
	b.l	print_char
	add	r29, r29, #8
	ldw	r4, [r29+], #-4
	ldw	r5, [r29+], #-8
	add	r5, r5, #1
	b	.L6
.L5:
	stw	r4, [r29+], #-4
	sub	r29, r29, #4
	mov	r0, #10
	b.l	print_char
	add	r29, r29, #4
	ldw	r4, [r29+], #-4
	stw	r4, [r29+], #-4
	sub	r29, r29, #4
	mov	r0, #10
	b.l	print_char
	add	r29, r29, #4
	ldw	r4, [r29+], #-4
	mov	r5, #0
.L14:
	cmpsub.a	r5, #128
	beg	.L8
.L7:
	add	r6, r5, #1
	mov	r9, r6
.L13:
	cmpsub.a	r9, #128
	beg	.L10
.L9:
	ldw	r3, [r4+], r9<<#2
	mov	r7, r3
	ldw	r3, [r4+], r5<<#2
	mov	r6, r3
	cmpsub.a	r7, r6
	beg	.L12
.L11:
	ldw	r3, [r4+], r5<<#2
	mov	r6, r3
	mov	r8, r6
	ldw	r3, [r4+], r9<<#2
	mov	r6, r3
	stw	r6, [r4+], r5<<#2
	stw	r8, [r4+], r9<<#2
.L12:
	add	r6, r9, #1
	mov	r9, r6
	b	.L13
.L10:
	add	r5, r5, #1
	b	.L14
.L8:
	mov	r5, #0
.L17:
	cmpsub.a	r5, #128
	beg	.L16
.L15:
	ldw	r3, [r4+], r5<<#2
	mov	r6, r3
	stw	r4, [r29+], #-4
	stw	r5, [r29+], #-8
	sub	r29, r29, #8
	mov	r0, r6
	b.l	print_int
	add	r29, r29, #8
	ldw	r4, [r29+], #-4
	ldw	r5, [r29+], #-8
	stw	r4, [r29+], #-4
	stw	r5, [r29+], #-8
	sub	r29, r29, #8
	mov	r0, #10
	b.l	print_char
	add	r29, r29, #8
	ldw	r4, [r29+], #-4
	ldw	r5, [r29+], #-8
	add	r5, r5, #1
	b	.L17
.L16:
	b.l	print_ln
	b.l	print_ln
	mov	r0, #0
.L18:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
