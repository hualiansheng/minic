	.file	"00simple.c"
	.text
	.align	2
	.global	main
	.type	main,function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldw	r15, .L3
	ldw	r0, [r15+], #0
	ldw	r15, .L3+4
	ldw	r15, [r15+], #0
	add	r0, r0, r15
	@ lr needed for prologue
	ldw	r15, .L3+8
	ldw	r15, [r15+], #0
	add	r0, r0, r15
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
