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
	@r1 = 0x80000000
	ldw	r1, .L3
	ldw	r1, [r1+], #0
	ldw	r0, .L3+4
	ldw	r0, [r0+], #0
	mov r2,#0
	
	add.a r3,r2,r0<<#1
	@r3 = 0xfffffffe
	@C=0

	add.a r4,r2,r0<<#2
	@r4 = 0xfffffffc
	@C=0
	
	add.a r5,r2,r0<<#31
	@r5 = 0x80000000
	@C=0,Z=0

	add.a r6,r2,r1>>#1
	@r6 = 0x4000000
	@C=0

	add.a r7,r2,r1>>#31
	@r7 = 1
	@C = 0

	add.a r8,r2,r0>>#1
	@r8 = 0x3fffffff 
	@C=0
	
	or.a r9,r2,r0>>#1
	@r9 = 0x3fffffff
	@C=1
	
	or.a r10,r2,r0<>#31
	@r10 = 0xfffffffe
	@C=1
	
	or.a r11,r2,r1<>#31
	@r11 = 1
	@C=0
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
	.word	2147483648
	.global	arg3
	.align	2
	.type	arg3, object
	.size	arg3, 4
arg3:
	.word	-1
	.ident	"GCC: (UC4_1.0_gama_20101126) 4.4.2"
