	.file	"00simple.c"
	.text
	.align	2
	.global	main
	.type	main,function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	@r0 = 0x7fffffff
	@r1 = 0x00070001
	ldw	r1, .L3
	ldw	r1, [r1+], #0
	ldw	r0, .L3+4
	ldw	r0, [r0+], #0
	and r2, r0, r1
	@r2 = 0x70001
	xor r3, r0, r1
	@r3 = 0x7ff8fffe
	sub r4, r0, r1
	@r4 = 0x7ff8fffe
	rsub r5, r0, r1
	@r5 = 0x80070002
	add r6, r0, r1
	@r6 = 0x80070000
	or r7, r0, r1
	@r7 = 0x7fffffff
	@clb r8, r0, r1
	@r8 = 0x7ff8fffe
	@movn r9, r0
	@r9 = 0x80000000
	@;;;;
	ldw r0, .L3+8
	ldw r0, [r0+], #0
	@r0 = 0xffffffff
	add.a r10, r0,#1
	@r10 = 0x0
	addc r10,r10,#0
	@r10 = 0x1
	mov r11, #0
	sub.a r11,#1
	@r11 = 0xffffffff
	subc r11,r11,#0
	@r11 = 0xffffffff
	mov r12, #0
	sub r12, #1
	rsubc r12,r12,#0
	@r12 = 0x1
	@;;;;
	@cmpand r0,#0
	@mov r1,#1
	@cmpxor r1,#1
	@cmpsub r1,#1
	@cmpadd r0,#1

	jump	lr
.L4:
	.align	2
.L3:
	.word	arg2
	.word	arg1
	.word	arg3
	.size	main, .-main
	.global	arg1
	.data
	.align	2
	.type	arg1, object
	.size	arg1, 4
arg1:
	.word	2147483647
	.global	arg2
	.align	2
	.type	arg2, object
	.size	arg2, 4
arg2:
	.word	458753
	.global	arg3
	.align	2
	.type	arg3, object
	.size	arg3, 4
arg3:
	.word	-1
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
