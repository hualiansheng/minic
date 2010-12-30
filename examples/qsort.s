	.text
	.global	qsort
	.type	qsort,function
qsort:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #52
	stw	r17, [r27+], #-16
	stw	r18, [r27+], #-20
	stw	r19, [r27+], #-24
	stw	r0, [r27+], #-28
	stw	r1, [r27+], #-32
	mov	r19, r2
.L1:
	ldw	r1, [r27+], #-32
	add	r5, r1, #1
	cmpsub.a	r19, r5
	bsg	.L3
.L2:
	b	.L9
.L3:
	mov	r18, r5
	mov	r17, r5
.L8:
	cmpsub.a	r17, r19
	beg	.L5
.L4:
	ldw	r1, [r27+], #-28
	ldw	r6, [r1+], r17<<#2
	ldw	r1, [r27+], #-28
	ldw	r2, [r27+], #-32
	ldw	r5, [r1+], r2<<#2
	cmpsub.a	r6, r5
	beg	.L7
.L6:
	ldw	r1, [r27+], #-28
	ldw	r6, [r1+], r18<<#2
	ldw	r1, [r27+], #-28
	add	r5, r1, r18<<#2
	ldw	r1, [r27+], #-28
	ldw	r4, [r1+], r17<<#2
	stw	r4, [r5+], #0
	ldw	r1, [r27+], #-28
	stw	r6, [r1+], r17<<#2
	add	r18, r18, #1
.L7:
	add	r17, r17, #1
	b	.L8
.L5:
	ldw	r1, [r27+], #-28
	ldw	r2, [r27+], #-32
	ldw	r6, [r1+], r2<<#2
	ldw	r1, [r27+], #-28
	ldw	r2, [r27+], #-32
	add	r17, r1, r2<<#2
	ldw	r1, [r27+], #-28
	sub	r2, r18, #1
	ldw	r5, [r1+], r2<<#2
	stw	r5, [r17+], #0
	ldw	r1, [r27+], #-28
	stw	r6, [r1+], r2<<#2
	ldw	r0, [r27+], #-28
	ldw	r1, [r27+], #-32
	b.l	qsort
	ldw	r28, [r27+], #-28
	mov	r3, r18
	stw	r3, [r27+], #-32
	b	.L1
.L9:
	ldw	r17, [r27+], #-16
	ldw	r18, [r27+], #-20
	ldw	r19, [r27+], #-24
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
	stw	r17, [r27+], #-16
	ldw	r28, .L22+0
	sub	r29, r29, #540
	add	r3, r27, r28
	stw	r3, [r27+], #-536
.L11:
	ldw	r4, [r27+], #-536
	mov	r5, #0
.L14:
	cmpsub.a	r5, #128
	beg	.L13
.L12:
	rsub	r6, r5, #128
	stw	r6, [r4+], r5<<#2
	add	r5, r5, #1
	b	.L14
.L13:
	mov	r5, #0
.L17:
	cmpsub.a	r5, #128
	beg	.L16
.L15:
	ldw	r0, [r4+], r5<<#2
	stw	r4, [r27+], #-536
	stw	r5, [r27+], #-20
	b.l	print_int
	ldw	r4, [r27+], #-536
	ldw	r5, [r27+], #-20
	stw	r4, [r27+], #-536
	stw	r5, [r27+], #-20
	mov	r0, #32
	b.l	print_char
	ldw	r4, [r27+], #-536
	ldw	r5, [r27+], #-20
	add	r5, r5, #1
	b	.L17
.L16:
	stw	r4, [r27+], #-536
	mov	r0, #10
	b.l	print_char
	ldw	r4, [r27+], #-536
	stw	r4, [r27+], #-536
	mov	r0, #10
	b.l	print_char
	ldw	r0, [r27+], #-536
	stw	r0, [r27+], #-536
	mov	r1, #0
	mov	r2, #128
	b.l	qsort
	ldw	r4, [r27+], #-536
	mov	r5, #0
.L20:
	cmpsub.a	r5, #128
	beg	.L19
.L18:
	ldw	r0, [r4+], r5<<#2
	stw	r4, [r27+], #-536
	stw	r5, [r27+], #-20
	b.l	print_int
	ldw	r4, [r27+], #-536
	ldw	r5, [r27+], #-20
	stw	r4, [r27+], #-536
	stw	r5, [r27+], #-20
	mov	r0, #32
	b.l	print_char
	ldw	r4, [r27+], #-536
	ldw	r5, [r27+], #-20
	add	r5, r5, #1
	b	.L20
.L19:
	mov	r0, #10
	b.l	print_char
	mov	r0, #10
	b.l	print_char
	mov	r0, #0
.L21:
	ldw	r17, [r27+], #-16
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L22:
	.word	-532
