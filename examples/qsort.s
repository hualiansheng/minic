	.global	qsort
	.type qsort, function
qsort:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #40
	stw	r0, [r27+], #-24
	stw	r1, [r27+], #-20
	stw	r2, [r27+], #-16
	ldw	r8, [r27+], #-16
	ldw	r7, [r27+], #-20
	ldw	r6, [r27+], #-24
	add	r4, r7, #1
	cmpsub.a	r8, r4
	bsg	.L2
.L1:
	b	.L8
.L2:
	add	r4, r7, #1
	mov	r5, r4
	add	r4, r7, #1
.L7:
	cmpsub.a	r4, r8
	beg	.L4
	ldw	r11, [r27+], #-36
.L3:
	ldw	r3, [r6+], r4<<#2
	mov	r10, r3
	ldw	r3, [r6+], r7<<#2
	mov	r9, r3
	cmpsub.a	r10, r9
	beg	.L6
.L5:
	ldw	r3, [r6+], r5<<#2
	mov	r9, r3
	mov	r11, r9
	ldw	r3, [r6+], r4<<#2
	mov	r9, r3
	stw	r9, [r6+], r5<<#2
	stw	r11, [r6+], r4<<#2
	add	r5, r5, #1
.L6:
	add	r4, r4, #1
	b	.L7
.L4:
	ldw	r3, [r6+], r7<<#2
	mov	r9, r3
	mov	r11, r9
	sub	r9, r5, #1
	ldw	r3, [r6+], r9<<#2
	mov	r9, r3
	stw	r9, [r6+], r7<<#2
	sub	r9, r5, #1
	stw	r11, [r6+], r9<<#2
	sub	r9, r5, #1
	stw	r4, [r29+], #-4
	stw	r5, [r29+], #-8
	stw	r6, [r29+], #-12
	stw	r7, [r29+], #-16
	stw	r8, [r29+], #-20
	stw	r11, [r29+], #-24
	sub	r29, r29, #24
	mov	r0, r6
	mov	r1, r7
	mov	r2, r9
	b.l	qsort
	add	r29, r29, #24
	ldw	r4, [r29+], #-4
	ldw	r5, [r29+], #-8
	ldw	r6, [r29+], #-12
	ldw	r7, [r29+], #-16
	ldw	r8, [r29+], #-20
	ldw	r11, [r29+], #-24
	stw	r4, [r29+], #-4
	stw	r5, [r29+], #-8
	stw	r6, [r29+], #-12
	stw	r7, [r29+], #-16
	stw	r8, [r29+], #-20
	stw	r11, [r29+], #-24
	sub	r29, r29, #24
	mov	r0, r6
	mov	r1, r5
	mov	r2, r8
	b.l	qsort
	add	r29, r29, #24
	ldw	r4, [r29+], #-4
	ldw	r5, [r29+], #-8
	ldw	r6, [r29+], #-12
	ldw	r7, [r29+], #-16
	ldw	r8, [r29+], #-20
	ldw	r11, [r29+], #-24
.L8:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
	.global	main
	.type main, function
main:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #536
	add	r3, r27, #-528
	stw	r3, [r27+], #-532
	ldw	r5, [r27+], #-532
	mov	r7, #0
.L11:
	cmpsub.a	r7, #128
	beg	.L10
.L9:
	stw	r5, [r29+], #-4
	stw	r7, [r29+], #-8
	sub	r29, r29, #8
	b.l	gen_rand
	add	r29, r29, #8
	ldw	r5, [r29+], #-4
	ldw	r7, [r29+], #-8
	mov	r4, r0
	stw	r4, [r5+], r7<<#2
	add	r4, r7, #1
	mov	r7, r4
	b	.L11
.L10:
	stw	r5, [r29+], #-4
	stw	r7, [r29+], #-8
	sub	r29, r29, #8
	mov	r0, r5
	mov	r1, #0
	mov	r2, #128
	b.l	qsort
	add	r29, r29, #8
	ldw	r5, [r29+], #-4
	ldw	r7, [r29+], #-8
	mov	r7, #0
.L14:
	cmpsub.a	r7, #128
	beg	.L13
.L12:
	ldw	r3, [r5+], r7<<#2
	mov	r4, r3
	stw	r5, [r29+], #-4
	stw	r7, [r29+], #-8
	sub	r29, r29, #8
	mov	r0, r4
	b.l	print_int
	add	r29, r29, #8
	ldw	r5, [r29+], #-4
	ldw	r7, [r29+], #-8
	stw	r5, [r29+], #-4
	stw	r7, [r29+], #-8
	sub	r29, r29, #8
	b.l	print_blank
	add	r29, r29, #8
	ldw	r5, [r29+], #-4
	ldw	r7, [r29+], #-8
	add	r4, r7, #1
	mov	r7, r4
	b	.L14
.L13:
	mov	r0, #0
.L15:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
