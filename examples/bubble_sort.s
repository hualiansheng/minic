	.text
	.global	main
	.type	main,function
main:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	ldw	r28, .L20+0
	sub	r27, r29, #4
	sub	r29, r29, #544
	add	r3, r27, r28
	stw	r3, [r27+], #-532
.L1:
	ldw	r4, [r27+], #-532
	mov	r5, #0
.L4:
	cmpsub.a	r5, #128
	beg	.L3
.L2:
	add	r7, r4, r5<<#2
	stw	r4, [r27+], #-532
	stw	r5, [r27+], #-16
	stw	r7, [r29+], #-4
	sub	r29, r29, #4
	mov	r0, #128
	b.l	rand_int
	ldw	r4, [r27+], #-532
	ldw	r5, [r27+], #-16
	add	r29, r29, #4
	ldw	r7, [r29+], #-4
	mov	r6, r0
	stw	r6, [r7+], #0
	add	r5, r5, #1
	b	.L4
.L3:
	mov	r5, #0
.L7:
	cmpsub.a	r5, #128
	beg	.L6
.L5:
	ldw	r9, [r27+], #-536
	ldw	r8, [r27+], #-540
	ldw	r0, [r4+], r5<<#2
	stw	r4, [r27+], #-532
	stw	r5, [r27+], #-16
	b.l	print_int
	ldw	r4, [r27+], #-532
	ldw	r5, [r27+], #-16
	stw	r4, [r27+], #-532
	stw	r5, [r27+], #-16
	mov	r0, #10
	b.l	print_char
	ldw	r4, [r27+], #-532
	ldw	r5, [r27+], #-16
	add	r5, r5, #1
	b	.L7
.L6:
	stw	r4, [r27+], #-532
	mov	r0, #10
	b.l	print_char
	ldw	r4, [r27+], #-532
	stw	r4, [r27+], #-532
	mov	r0, #10
	b.l	print_char
	ldw	r4, [r27+], #-532
	mov	r5, #0
.L15:
	cmpsub.a	r5, #128
	beg	.L9
.L8:
	add	r10, r5, #1
	mov	r9, r10
.L14:
	cmpsub.a	r9, #128
	beg	.L11
.L10:
	ldw	r7, [r4+], r9<<#2
	ldw	r6, [r4+], r5<<#2
	cmpsub.a	r7, r6
	beg	.L13
.L12:
	ldw	r8, [r4+], r5<<#2
	ldw	r6, [r4+], r9<<#2
	stw	r6, [r4+], r5<<#2
	stw	r8, [r4+], r9<<#2
.L13:
	add	r9, r9, #1
	b	.L14
.L11:
	mov	r5, r10
	b	.L15
.L9:
	mov	r5, #0
.L18:
	cmpsub.a	r5, #128
	beg	.L17
.L16:
	ldw	r0, [r4+], r5<<#2
	stw	r4, [r27+], #-532
	stw	r5, [r27+], #-16
	b.l	print_int
	ldw	r4, [r27+], #-532
	ldw	r5, [r27+], #-16
	stw	r4, [r27+], #-532
	stw	r5, [r27+], #-16
	mov	r0, #10
	b.l	print_char
	ldw	r4, [r27+], #-532
	ldw	r5, [r27+], #-16
	add	r5, r5, #1
	b	.L18
.L17:
	b.l	print_ln
	b.l	print_ln
	mov	r0, #0
.L19:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
.L20:
	.word	-528
