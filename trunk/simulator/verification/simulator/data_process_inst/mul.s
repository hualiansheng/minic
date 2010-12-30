	.file	"00simple.c"
	.text
	.align	2
	.global	main
	.type	main,function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	@r0 = 0x04000000
	@r1 = 0x40000000
	ldw	r1, .L3
	ldw	r1, [r1+], #0
	ldw	r0, .L3+4
	ldw	r0, [r0+], #0
	mov r2,#2
	mul.a r2, r1, r2
	@N = 1, Z = 0
	@r2 = 0x80000000
	mov r3,#-2
	mul.a r3, r3, r0
	@N = 1, Z = 0
	@r3 = 0xf8000000
	mov r4,#0
	mul.a r4, r3, r4
	@Z = 1, N = 0
	@r4=0

	mula.a r5, r3, r4, r0
	@r5 = 0x04000000
	@N = 0, Z = 0
	
	sub r1,r1,#1
	mov r6,#2
	mula.a r6,r1,r6,r6
	@N = 1, Z = 0
	@r6 = 0x8000000

	mov r7,#-1 
	mov r7,r7>>#1
	mov r8,#2
	mula.a r7,r7,r8,r8
	@r7 = 0
	@N = 0, Z = 1
	;r2 = 255
	;r3 = 0
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
	.word	67108864
	.global	arg2
	.align	2
	.type	arg2, object
	.size	arg2, 4
arg2:
	.word	1073741824
	.global	arg3
	.align	2
	.type	arg3, object
	.size	arg3, 4
arg3:
	.word	-1
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
