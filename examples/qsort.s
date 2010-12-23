	.text
	.global	qsort
	.type	qsort,function
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
	add	r9, r6, r4<<#2
	ldw	r10, [r9+], #0
	add	r9, r6, r7<<#2
	ldw	r9, [r9+], #0
	cmpsub.a	r10, r9
	beg	.L6
.L5:
	add	r9, r6, r5<<#2
	ldw	r9, [r9+], #0
	mov	r11, r9
	add	r10, r6, r5<<#2
	add	r9, r6, r4<<#2
	ldw	r9, [r9+], #0
	stw	r9, [r10+], #0
	add	r9, r6, r4<<#2
	stw	r11, [r9+], #0
	add	r5, r5, #1
.L6:
	add	r4, r4, #1
	b	.L7
.L4:
	add	r9, r6, r7<<#2
	ldw	r9, [r9+], #0
	mov	r11, r9
	add	r10, r6, r7<<#2
	sub	r9, r5, #1
	add	r9, r6, r9<<#2
	ldw	r9, [r9+], #0
	stw	r9, [r10+], #0
	sub	r9, r5, #1
	add	r9, r6, r9<<#2
	stw	r11, [r9+], #0
	sub	r9, r5, #1
	stw	r4, [r27+], #-32
	stw	r5, [r27+], #-28
	stw	r6, [r27+], #-24
	stw	r7, [r27+], #-20
	stw	r8, [r27+], #-16
	stw	r11, [r27+], #-36
	mov	r0, r6
	mov	r1, r7
	mov	r2, r9
	b.l	qsort
	ldw	r8, [r27+], #-16
	ldw	r7, [r27+], #-20
	ldw	r6, [r27+], #-24
	ldw	r5, [r27+], #-28
	ldw	r4, [r27+], #-32
	ldw	r11, [r27+], #-36
	mov	r0, r6
	mov	r1, r5
	mov	r2, r8
	b.l	qsort
.L8:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L9:
	.text
	.global	main
	.type	main,function
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
.L12:
	cmpsub.a	r7, #128
	beg	.L11
.L10:
	add	r6, r5, r7<<#2
	rsub	r4, r7, #128
	stw	r4, [r6+], #0
	add	r4, r7, #1
	mov	r7, r4
	b	.L12
.L11:
	mov	r7, #0
.L15:
	cmpsub.a	r7, #128
	beg	.L14
.L13:
	add	r4, r5, r7<<#2
	ldw	r4, [r4+], #0
	stw	r5, [r27+], #-532
	stw	r7, [r27+], #-16
	mov	r0, r4
	b.l	print_int
	ldw	r7, [r27+], #-16
	ldw	r5, [r27+], #-532
	stw	r5, [r27+], #-532
	stw	r7, [r27+], #-16
	mov	r0, #32
	b.l	print_char
	ldw	r7, [r27+], #-16
	ldw	r5, [r27+], #-532
	add	r4, r7, #1
	mov	r7, r4
	b	.L15
.L14:
	stw	r5, [r27+], #-532
	stw	r7, [r27+], #-16
	mov	r0, #10
	b.l	print_char
	ldw	r7, [r27+], #-16
	ldw	r5, [r27+], #-532
	stw	r5, [r27+], #-532
	stw	r7, [r27+], #-16
	mov	r0, #10
	b.l	print_char
	ldw	r7, [r27+], #-16
	ldw	r5, [r27+], #-532
	stw	r5, [r27+], #-532
	mov	r0, r5
	mov	r1, #0
	mov	r2, #128
	b.l	qsort
	ldw	r5, [r27+], #-532
	mov	r7, #0
.L18:
	cmpsub.a	r7, #128
	beg	.L17
.L16:
	add	r4, r5, r7<<#2
	ldw	r4, [r4+], #0
	stw	r5, [r27+], #-532
	stw	r7, [r27+], #-16
	mov	r0, r4
	b.l	print_int
	ldw	r7, [r27+], #-16
	ldw	r5, [r27+], #-532
	stw	r5, [r27+], #-532
	stw	r7, [r27+], #-16
	mov	r0, #32
	b.l	print_char
	ldw	r7, [r27+], #-16
	ldw	r5, [r27+], #-532
	add	r4, r7, #1
	mov	r7, r4
	b	.L18
.L17:
	mov	r0, #10
	b.l	print_char
	mov	r0, #10
	b.l	print_char
	mov	r0, #0
.L19:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L20:
