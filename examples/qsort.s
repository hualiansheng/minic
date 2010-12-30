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
	mov	r7, r0
	mov	r6, r1
	mov	r5, r2
.L1:
	add	r4, r6, #1
	cmpsub.a	r5, r4
	bsg	.L3
.L2:
	b	.L9
.L3:
	add	r4, r6, #1
	add	r11, r6, #1
.L8:
	cmpsub.a	r11, r5
	beg	.L5
.L4:
	ldw	r9, [r7+], r11<<#2
	ldw	r8, [r7+], r6<<#2
	cmpsub.a	r9, r8
	beg	.L7
.L6:
	ldw	r10, [r7+], r4<<#2
	ldw	r8, [r7+], r11<<#2
	stw	r8, [r7+], r4<<#2
	stw	r10, [r7+], r11<<#2
	add	r4, r4, #1
.L7:
	add	r11, r11, #1
	b	.L8
.L5:
	ldw	r10, [r7+], r6<<#2
	sub	r8, r4, #1
	ldw	r8, [r7+], r8<<#2
	stw	r8, [r7+], r6<<#2
	sub	r8, r4, #1
	stw	r10, [r7+], r8<<#2
	sub	r2, r4, #1
	stw	r4, [r27+], #-28
	stw	r5, [r27+], #-24
	stw	r7, [r27+], #-16
	mov	r0, r7
	mov	r1, r6
	b.l	qsort
	ldw	r1, [r27+], #-28
	ldw	r2, [r27+], #-24
	ldw	r0, [r27+], #-16
	b.l	qsort
.L9:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L10:
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
	ldw	r28, .L22+0
	add	r3, r27, r28
	stw	r3, [r27+], #-532
.L11:
	ldw	r5, [r27+], #-532
	mov	r7, #0
.L14:
	cmpsub.a	r7, #128
	beg	.L13
.L12:
	rsub	r4, r7, #128
	stw	r4, [r5+], r7<<#2
	add	r7, r7, #1
	b	.L14
.L13:
	mov	r7, #0
.L17:
	cmpsub.a	r7, #128
	beg	.L16
.L15:
	ldw	r0, [r5+], r7<<#2
	stw	r5, [r27+], #-532
	stw	r7, [r27+], #-16
	b.l	print_int
	ldw	r5, [r27+], #-532
	ldw	r7, [r27+], #-16
	stw	r5, [r27+], #-532
	stw	r7, [r27+], #-16
	mov	r0, #32
	b.l	print_char
	ldw	r5, [r27+], #-532
	ldw	r7, [r27+], #-16
	add	r7, r7, #1
	b	.L17
.L16:
	stw	r5, [r27+], #-532
	mov	r0, #10
	b.l	print_char
	ldw	r5, [r27+], #-532
	stw	r5, [r27+], #-532
	mov	r0, #10
	b.l	print_char
	ldw	r0, [r27+], #-532
	stw	r0, [r27+], #-532
	mov	r1, #0
	mov	r2, #128
	b.l	qsort
	ldw	r5, [r27+], #-532
	mov	r7, #0
.L20:
	cmpsub.a	r7, #128
	beg	.L19
.L18:
	ldw	r0, [r5+], r7<<#2
	stw	r5, [r27+], #-532
	stw	r7, [r27+], #-16
	b.l	print_int
	ldw	r5, [r27+], #-532
	ldw	r7, [r27+], #-16
	stw	r5, [r27+], #-532
	stw	r7, [r27+], #-16
	mov	r0, #32
	b.l	print_char
	ldw	r5, [r27+], #-532
	ldw	r7, [r27+], #-16
	add	r7, r7, #1
	b	.L20
.L19:
	mov	r0, #10
	b.l	print_char
	mov	r0, #10
	b.l	print_char
	mov	r0, #0
.L21:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L22:
	.word	-528
