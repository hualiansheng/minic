	.text
	.global	qsort
	.type	qsort,function
qsort:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #44
	mov	r4, r0
	mov	r5, r1
	mov	r6, r2
.L1:
	add	r8, r5, #1
	cmpsub.a	r6, r8
	bsg	.L3
.L2:
	mov	r0, #0
	b	.L9
.L3:
	mov	r13, r8
	mov	r12, r8
.L8:
	cmpsub.a	r12, r6
	beg	.L5
	ldw	r11, [r27+], #-40
.L4:
	add	r9, r4, r12<<#2
	ldw	r8, [r9+], #0
	add	r7, r4, r5<<#2
	ldw	r7, [r7+], #0
	cmpsub.a	r8, r7
	beg	.L7
.L6:
	add	r8, r4, r13<<#2
	ldw	r7, [r8+], #0
	mov	r11, r7
	ldw	r7, [r9+], #0
	stw	r7, [r8+], #0
	stw	r11, [r9+], #0
	add	r7, r13, #1
	mov	r13, r7
.L7:
	add	r7, r12, #1
	mov	r12, r7
	b	.L8
.L5:
	add	r10, r4, r5<<#2
	ldw	r7, [r10+], #0
	mov	r11, r7
	sub	r9, r13, #1
	add	r8, r4, r9<<#2
	ldw	r7, [r8+], #0
	stw	r7, [r10+], #0
	stw	r11, [r8+], #0
	stw	r4, [r27+], #-28
	stw	r5, [r27+], #-24
	stw	r6, [r27+], #-20
	stw	r11, [r27+], #-40
	stw	r12, [r27+], #-36
	stw	r13, [r27+], #-32
	mov	r0, r4
	mov	r1, r5
	mov	r2, r9
	b.l	qsort
	ldw	r6, [r27+], #-20
	ldw	r5, [r27+], #-24
	ldw	r4, [r27+], #-28
	ldw	r13, [r27+], #-32
	ldw	r12, [r27+], #-36
	ldw	r11, [r27+], #-40
	mov	r5, r13
	b	.L1
	mov	r0, #0
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
	add	r3, r27, #-528
	stw	r3, [r27+], #-532
	ldw	r5, [r27+], #-532
.L11:
	mov	r7, #0
.L14:
	cmpsub.a	r7, #128
	beg	.L13
.L12:
	add	r6, r5, r7<<#2
	rsub	r4, r7, #128
	stw	r4, [r6+], #0
	add	r4, r7, #1
	mov	r7, r4
	b	.L14
.L13:
	mov	r7, #0
.L17:
	cmpsub.a	r7, #128
	beg	.L16
.L15:
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
	b	.L17
.L16:
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
.L20:
	cmpsub.a	r7, #128
	beg	.L19
.L18:
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
