	.global	mod
	.type	mod,function
mod:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #24
	stw	r0, [r27+], #-20
	stw	r1, [r27+], #-16
	ldw	r6, [r27+], #-16
	ldw	r5, [r27+], #-20
.L3:
	cmpsub.a	r5, r6
	bsl	.L2
.L1:
	sub	r4, r5, r6
	mov	r5, r4
	b	.L3
.L2:
	mov	r0, r5
.L4:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
	.global	div
	.type	div,function
div:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #28
	stw	r0, [r27+], #-20
	stw	r1, [r27+], #-16
	ldw	r7, [r27+], #-16
	ldw	r6, [r27+], #-20
	mov	r5, #0
.L7:
	cmpsub.a	r6, r7
	bsl	.L6
.L5:
	sub	r4, r6, r7
	mov	r6, r4
	add	r4, r5, #1
	mov	r5, r4
	b	.L7
.L6:
	mov	r0, r5
.L8:
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30
	.global	main
	.type	main,function
main:
	stw	r31, [r29+], #-4
	stw	r30, [r29+], #-8
	stw	r29, [r29+], #-12
	stw	r27, [r29+], #-16
	sub	r27, r29, #4
	sub	r29, r29, #388
	stw	r17, [r27+], #-16
	add	r3, r27, #-108
	stw	r3, [r27+], #-112
	add	r3, r27, #-192
	stw	r3, [r27+], #-196
	add	r3, r27, #-356
	stw	r3, [r27+], #-360
	ldw	r10, [r27+], #-112
	ldw	r9, [r27+], #-196
	ldw	r8, [r27+], #-360
	mov	r3, #1
	stw	r3, [r10+], #0
	mov	r3, #1
	stw	r3, [r9+], #0
	mov	r3, #0
	stw	r3, [r8+], #0
	mov	r7, #10
	mov	r6, #0
.L11:
	cmpsub.a	r6, #40
	beg	.L10
.L9:
	mov	r3, #0
	stw	r3, [r8+], r6<<#2
	add	r4, r6, #1
	mov	r6, r4
	b	.L11
.L10:
	mov	r6, #0
.L14:
	cmpsub.a	r6, r7
	beg	.L13
.L12:
	mov	r3, #11
	stw	r3, [r10+], r6<<#2
	add	r4, r6, #1
	mov	r6, r4
	b	.L14
.L13:
	mov	r5, #8
	mov	r6, #0
.L17:
	cmpsub.a	r6, r5
	beg	.L16
.L15:
	mov	r3, #5
	stw	r3, [r9+], r6<<#2
	add	r4, r6, #1
	mov	r6, r4
	b	.L17
.L16:
	mov	r6, #0
.L23:
	cmpsub.a	r6, r7
	beg	.L19
.L18:
	mov	r4, #0
.L22:
	cmpsub.a	r4, r5
	beg	.L21
	ldw	r13, [r27+], #-380
	ldw	r11, [r27+], #-384
.L20:
	add	r12, r6, r4
	add	r12, r6, r4
	ldw	r3, [r8+], r12<<#2
	mov	r15, r3
	ldw	r3, [r10+], r6<<#2
	mov	r14, r3
	ldw	r3, [r9+], r4<<#2
	mov	r11, r3
	mul	r11, r14, r11
	add	r11, r15, r11
	stw	r11, [r8+], r12<<#2
	add	r4, r4, #1
	b	.L22
.L21:
	add	r4, r6, #1
	mov	r6, r4
	b	.L23
.L19:
	mov	r13, #0
	mov	r6, #0
.L26:
	add	r4, r7, r5
	cmpsub.a	r6, r4
	beg	.L25
	ldw	r4, [r27+], #-376
	ldw	r11, [r27+], #-384
.L24:
	ldw	r3, [r8+], r6<<#2
	mov	r4, r3
	add	r4, r4, r13
	mov	r11, r4
	stw	r5, [r29+], #-4
	stw	r6, [r29+], #-8
	sub	r29, r29, #8
	mov	r0, r11
	mov	r1, #16
	b.l	mod
	add	r29, r29, #8
	ldw	r5, [r29+], #-4
	ldw	r6, [r29+], #-8
	mov	r4, r0
	stw	r4, [r8+], r6<<#2
	stw	r5, [r29+], #-4
	stw	r6, [r29+], #-8
	stw	r7, [r29+], #-12
	sub	r29, r29, #12
	mov	r0, r11
	mov	r1, #16
	b.l	div
	add	r29, r29, #12
	ldw	r5, [r29+], #-4
	ldw	r6, [r29+], #-8
	ldw	r7, [r29+], #-12
	mov	r4, r0
	mov	r13, r4
	add	r4, r6, #1
	mov	r6, r4
	b	.L26
.L25:
	mov	r0, #0
.L27:
	ldw	r17, [r27+], #-16
	ldw	r30, [r27+], #-4
	ldw	r29, [r27+], #-8
	ldw	r27, [r27+], #-12
	jump	r30

