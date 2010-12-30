
a.out:     file format elf32-littleunicore32


Disassembly of section .init:

0200025c <_init>:
 200025c:	24ef4004 	sub	sp, sp, #4	; 0x4
 2000260:	78ef8000 	stw	lr, [sp+]
 2000264:	bd000015 	b.l	20002bc <call_gmon_start>
 2000268:	bd000056 	b.l	20003c4 <frame_dummy>
 200026c:	bd0000b4 	b.l	2000540 <__do_global_ctors_aux>
 2000270:	79ef0000 	ldw	ip, [sp+]
 2000274:	28ef4004 	add	sp, sp, #4	; 0x4
 2000278:	10ffc13c 	jump	ip

Disassembly of section .text:

0200027c <_start>:
 200027c:	79ff0030 	ldw	ip, [pc+], #48	; 20002b0 <_start+0x34>
 2000280:	3a06c000 	mov	fp, #0	; 0x0
 2000284:	69e84004 	ldw.w	r1, [sp]+, #4
 2000288:	1a00801d 	mov	r2, sp
 200028c:	72e88004 	stw.w	r2, [sp-], #4
 2000290:	72e80004 	stw.w	r0, [sp-], #4
 2000294:	1a000000 	nop			(mov r0,r0)
 2000298:	1a000000 	nop			(mov r0,r0)
 200029c:	79f80014 	ldw	r0, [pc+], #20	; 20002b4 <_start+0x38>
 20002a0:	79f8c014 	ldw	r3, [pc+], #20	; 20002b8 <_start+0x3c>
 20002a4:	72ef0004 	stw.w	ip, [sp-], #4
 20002a8:	bd0000d0 	b.l	20005ec <_fini+0x44>
 20002ac:	bd0000c9 	b.l	20005d4 <_fini+0x2c>
 20002b0:	0200047c 	.word	0x0200047c
 20002b4:	0200041c 	.word	0x0200041c
 20002b8:	02000480 	.word	0x02000480

020002bc <call_gmon_start>:
 20002bc:	24ef4004 	sub	sp, sp, #4	; 0x4
 20002c0:	78ee8000 	stw	sl, [sp+]
 20002c4:	79fe802c 	ldw	sl, [pc+], #44	; 20002f4 <call_gmon_start+0x38>
 20002c8:	08fe801a 	add	sl, pc, sl
 20002cc:	79fbc028 	ldw	r15, [pc+], #40	; 20002f8 <call_gmon_start+0x3c>
 20002d0:	59d3c00f 	ldw	r15, [sl+],r15
 20002d4:	35780000 	cmpsub.a	r15, #0	; 0x0
 20002d8:	a0000003 	beq	20002e8 <call_gmon_start+0x2c>
 20002dc:	79ee8000 	ldw	sl, [sp+]
 20002e0:	28ef4004 	add	sp, sp, #4	; 0x4
 20002e4:	bc0000be 	b	20005e0 <_fini+0x38>
 20002e8:	79ee8000 	ldw	sl, [sp+]
 20002ec:	28ef4004 	add	sp, sp, #4	; 0x4
 20002f0:	10ffc13e 	jump	lr
 20002f4:	00008488 	.word	0x00008488
 20002f8:	00000018 	.word	0x00000018

020002fc <__do_global_dtors_aux>:
 20002fc:	70ef8004 	stw	lr, [sp-], #4
 2000300:	70ee8008 	stw	sl, [sp-], #8
 2000304:	70ee000c 	stw	r24, [sp-], #12
 2000308:	70edc010 	stw	r23, [sp-], #16
 200030c:	70edc010 	stw	r23, [sp-], #16
 2000310:	70ed8014 	stw	r22, [sp-], #20
 2000314:	70edc010 	stw	r23, [sp-], #16
 2000318:	70ed8014 	stw	r22, [sp-], #20
 200031c:	24ef4014 	sub	sp, sp, #20	; 0x14
 2000320:	1a000000 	nop			(mov r0,r0)
 2000324:	79fd8078 	ldw	r22, [pc+], #120	; 20003a0 <__do_global_dtors_aux+0xa4>
 2000328:	7db3c000 	ldb	r15, [r22+]
 200032c:	35780000 	cmpsub.a	r15, #0	; 0x0
 2000330:	a2000014 	bne	2000384 <__do_global_dtors_aux+0x88>
 2000334:	79fdc06c 	ldw	r23, [pc+], #108	; 20003a4 <__do_global_dtors_aux+0xa8>
 2000338:	79fe006c 	ldw	r24, [pc+], #108	; 20003a8 <__do_global_dtors_aux+0xac>
 200033c:	06be0018 	rsub	r24, r23, r24
 2000340:	79fe8068 	ldw	sl, [pc+], #104	; 20003ac <__do_global_dtors_aux+0xb0>
 2000344:	1a060498 	mov	r24, r24 |> #2
 2000348:	79d3c000 	ldw	r15, [sl+]
 200034c:	24c60001 	sub	r24, r24, #1	; 0x1
 2000350:	15780018 	cmpsub.a	r15, r24
 2000354:	a4000009 	bea	200037c <__do_global_dtors_aux+0x80>
 2000358:	287bc001 	add	r15, r15, #1	; 0x1
 200035c:	78d3c000 	stw	r15, [sl+]
 2000360:	1a000000 	nop			(mov r0,r0)
 2000364:	28ff8008 	add	lr, pc, #8	; 0x8
 2000368:	59bf040f 	ldw	ip, [r23+],r15 << #2
 200036c:	10ffc13c 	jump	ip
 2000370:	79d3c000 	ldw	r15, [sl+]
 2000374:	15780018 	cmpsub.a	r15, r24
 2000378:	a6fffff7 	bub	2000358 <__do_global_dtors_aux+0x5c>
 200037c:	3a03c001 	mov	r15, #1	; 0x1
 2000380:	7cb3c000 	stb	r15, [r22+]
 2000384:	69ed8004 	ldw.w	r22, [sp]+, #4
 2000388:	1a000000 	nop			(mov r0,r0)
 200038c:	69edc004 	ldw.w	r23, [sp]+, #4
 2000390:	69ee0004 	ldw.w	r24, [sp]+, #4
 2000394:	69ee8004 	ldw.w	sl, [sp]+, #4
 2000398:	69ef0004 	ldw.w	ip, [sp]+, #4
 200039c:	10ffc13c 	jump	ip
 20003a0:	02008784 	.word	0x02008784
 20003a4:	02008680 	.word	0x02008680
 20003a8:	02008684 	.word	0x02008684
 20003ac:	02008788 	.word	0x02008788

020003b0 <call___do_global_dtors_aux>:
 20003b0:	24ef4004 	sub	sp, sp, #4	; 0x4
 20003b4:	78ef8000 	stw	lr, [sp+]
 20003b8:	79ef0000 	ldw	ip, [sp+]
 20003bc:	28ef4004 	add	sp, sp, #4	; 0x4
 20003c0:	10ffc13c 	jump	ip

020003c4 <frame_dummy>:
 20003c4:	24ef4004 	sub	sp, sp, #4	; 0x4
 20003c8:	78ef8000 	stw	lr, [sp+]
 20003cc:	79f80030 	ldw	r0, [pc+], #48	; 2000400 <frame_dummy+0x3c>
 20003d0:	1a000000 	nop			(mov r0,r0)
 20003d4:	7903c000 	ldw	r15, [r0+]
 20003d8:	35780000 	cmpsub.a	r15, #0	; 0x0
 20003dc:	a0000005 	beq	20003f4 <frame_dummy+0x30>
 20003e0:	79fbc020 	ldw	r15, [pc+], #32	; 2000404 <frame_dummy+0x40>
 20003e4:	35780000 	cmpsub.a	r15, #0	; 0x0
 20003e8:	a0000002 	beq	20003f4 <frame_dummy+0x30>
 20003ec:	28ff8004 	add	lr, pc, #4	; 0x4
 20003f0:	10ffc12f 	jump	r15
 20003f4:	79ef0000 	ldw	ip, [sp+]
 20003f8:	28ef4004 	add	sp, sp, #4	; 0x4
 20003fc:	10ffc13c 	jump	ip
 2000400:	02008688 	.word	0x02008688
 2000404:	00000000 	.word	0x00000000

02000408 <call_frame_dummy>:
 2000408:	24ef4004 	sub	sp, sp, #4	; 0x4
 200040c:	78ef8000 	stw	lr, [sp+]
 2000410:	79ef0000 	ldw	ip, [sp+]
 2000414:	28ef4004 	add	sp, sp, #4	; 0x4
 2000418:	10ffc13c 	jump	ip

0200041c <main>:
 200041c:	1a07001d 	mov	ip, sp
 2000420:	70efc004 	stw	pc, [sp-], #4
 2000424:	70ef8008 	stw	lr, [sp-], #8
 2000428:	70ef000c 	stw	ip, [sp-], #12
 200042c:	70ef000c 	stw	ip, [sp-], #12
 2000430:	70eec010 	stw	fp, [sp-], #16
 2000434:	70ef000c 	stw	ip, [sp-], #12
 2000438:	70eec010 	stw	fp, [sp-], #16
 200043c:	24ef4010 	sub	sp, sp, #16	; 0x10
 2000440:	24e6c004 	sub	fp, ip, #4	; 0x4
 2000444:	24ef4004 	sub	sp, sp, #4	; 0x4
 2000448:	79fbc028 	ldw	r15, [pc+], #40	; 2000474 <main+0x58>
 200044c:	70dbc010 	stw	r15, [fp-], #16
 2000450:	79fbc024 	ldw	r15, [pc+], #36	; 2000478 <main+0x5c>
 2000454:	1a000000 	nop			(mov r0,r0)
 2000458:	797bc000 	ldw	r15, [r15+]
 200045c:	1a00000f 	mov	r0, r15
 2000460:	1a07001b 	mov	ip, fp
 2000464:	71dec00c 	ldw	fp, [fp-], #12
 2000468:	71e74008 	ldw	sp, [ip-], #8
 200046c:	71e70004 	ldw	ip, [ip-], #4
 2000470:	10ffc13c 	jump	ip
 2000474:	02000600 	.word	0x02000600
 2000478:	02008780 	.word	0x02008780

0200047c <__libc_csu_fini>:
 200047c:	10ffc13e 	jump	lr

02000480 <__libc_csu_init>:
 2000480:	70ef8004 	stw	lr, [sp-], #4
 2000484:	70ee8008 	stw	sl, [sp-], #8
 2000488:	70ee000c 	stw	r24, [sp-], #12
 200048c:	70edc010 	stw	r23, [sp-], #16
 2000490:	70ed8014 	stw	r22, [sp-], #20
 2000494:	70ed4018 	stw	r21, [sp-], #24
 2000498:	70ed001c 	stw	r20, [sp-], #28
 200049c:	70ed001c 	stw	r20, [sp-], #28
 20004a0:	70ecc020 	stw	r19, [sp-], #32
 20004a4:	70ed001c 	stw	r20, [sp-], #28
 20004a8:	70ecc020 	stw	r19, [sp-], #32
 20004ac:	24ef4020 	sub	sp, sp, #32	; 0x20
 20004b0:	1a000000 	nop			(mov r0,r0)
 20004b4:	79fe807c 	ldw	sl, [pc+], #124	; 2000534 <__libc_csu_init+0xb4>
 20004b8:	08fe801a 	add	sl, pc, sl
 20004bc:	1a058000 	mov	r22, r0
 20004c0:	1a054001 	mov	r21, r1
 20004c4:	1a050002 	mov	r20, r2
 20004c8:	bdffff64 	b.l	200025c <_init>
 20004cc:	79fbc068 	ldw	r15, [pc+], #104	; 2000538 <__libc_csu_init+0xb8>
 20004d0:	59d4c00f 	ldw	r19, [sl+],r15
 20004d4:	79fbc064 	ldw	r15, [pc+], #100	; 200053c <__libc_csu_init+0xbc>
 20004d8:	59d5c00f 	ldw	r23, [sl+],r15
 20004dc:	069dc017 	rsub	r23, r19, r23
 20004e0:	1b05c497 	mov.a	r23, r23 |> #2
 20004e4:	a000000a 	beq	2000510 <__libc_csu_init+0x90>
 20004e8:	3a060000 	mov	r24, #0	; 0x0
 20004ec:	1a000016 	mov	r0, r22
 20004f0:	1a004015 	mov	r1, r21
 20004f4:	1a008014 	mov	r2, r20
 20004f8:	28ff8008 	add	lr, pc, #8	; 0x8
 20004fc:	599f0418 	ldw	ip, [r19+],r24 << #2
 2000500:	10ffc13c 	jump	ip
 2000504:	28c60001 	add	r24, r24, #1	; 0x1
 2000508:	15c00017 	cmpsub.a	r24, r23
 200050c:	a6fffff7 	bub	20004ec <__libc_csu_init+0x6c>
 2000510:	69ecc004 	ldw.w	r19, [sp]+, #4
 2000514:	69ed0004 	ldw.w	r20, [sp]+, #4
 2000518:	69ed4004 	ldw.w	r21, [sp]+, #4
 200051c:	69ed8004 	ldw.w	r22, [sp]+, #4
 2000520:	69edc004 	ldw.w	r23, [sp]+, #4
 2000524:	69ee0004 	ldw.w	r24, [sp]+, #4
 2000528:	69ee8004 	ldw.w	sl, [sp]+, #4
 200052c:	69ef0004 	ldw.w	ip, [sp]+, #4
 2000530:	10ffc13c 	jump	ip
 2000534:	00008298 	.word	0x00008298
 2000538:	00000020 	.word	0x00000020
 200053c:	0000001c 	.word	0x0000001c

02000540 <__do_global_ctors_aux>:
 2000540:	70ef8004 	stw	lr, [sp-], #4
 2000544:	70ef8004 	stw	lr, [sp-], #4
 2000548:	70ee8008 	stw	sl, [sp-], #8
 200054c:	70ef8004 	stw	lr, [sp-], #4
 2000550:	70ee8008 	stw	sl, [sp-], #8
 2000554:	24ef4008 	sub	sp, sp, #8	; 0x8
 2000558:	1a000000 	nop			(mov r0,r0)
 200055c:	79fe8030 	ldw	sl, [pc+], #48	; 2000590 <__do_global_ctors_aux+0x50>
 2000560:	71d3c004 	ldw	r15, [sl-], #4
 2000564:	37780001 	cmpadd.a	r15, #1	; 0x1
 2000568:	a0000006 	beq	2000584 <__do_global_ctors_aux+0x44>
 200056c:	24d68004 	sub	sl, sl, #4	; 0x4
 2000570:	28ff8004 	add	lr, pc, #4	; 0x4
 2000574:	10ffc12f 	jump	r15
 2000578:	73d3c004 	ldw.w	r15, [sl-], #4
 200057c:	37780001 	cmpadd.a	r15, #1	; 0x1
 2000580:	a2fffffb 	bne	2000570 <__do_global_ctors_aux+0x30>
 2000584:	69ee8004 	ldw.w	sl, [sp]+, #4
 2000588:	69ef0004 	ldw.w	ip, [sp]+, #4
 200058c:	10ffc13c 	jump	ip
 2000590:	0200867c 	.word	0x0200867c

02000594 <call___do_global_ctors_aux>:
 2000594:	24ef4004 	sub	sp, sp, #4	; 0x4
 2000598:	78ef8000 	stw	lr, [sp+]
 200059c:	79ef0000 	ldw	ip, [sp+]
 20005a0:	28ef4004 	add	sp, sp, #4	; 0x4
 20005a4:	10ffc13c 	jump	ip

Disassembly of section .fini:

020005a8 <_fini>:
 20005a8:	24ef4004 	sub	sp, sp, #4	; 0x4
 20005ac:	78ef8000 	stw	lr, [sp+]
 20005b0:	bdffff52 	b.l	20002fc <__do_global_dtors_aux>
 20005b4:	79ef0000 	ldw	ip, [sp+]
 20005b8:	28ef4004 	add	sp, sp, #4	; 0x4
 20005bc:	10ffc13c 	jump	ip

Disassembly of section .plt:

020005c0 <.plt>:
 20005c0:	72ef8004 	stw.w	lr, [sp-], #4
 20005c4:	79ff8008 	ldw	lr, [pc+], #8	; 20005d0 <_fini+0x28>
 20005c8:	08ff801e 	add	lr, pc, lr
 20005cc:	7bf7c00c 	ldw.w	pc, [lr+], #12
 20005d0:	00008184 	.word	0x00008184
 20005d4:	28ff1200 	add	ip, pc, #0	; 0x0
 20005d8:	28e72402 	add	ip, ip, #32768	; 0x8000
 20005dc:	7be7c188 	ldw.w	pc, [ip+], #392
 20005e0:	28ff1200 	add	ip, pc, #0	; 0x0
 20005e4:	28e72402 	add	ip, ip, #32768	; 0x8000
 20005e8:	7be7c180 	ldw.w	pc, [ip+], #384
 20005ec:	28ff1200 	add	ip, pc, #0	; 0x0
 20005f0:	28e72402 	add	ip, ip, #32768	; 0x8000
 20005f4:	7be7c178 	ldw.w	pc, [ip+], #376
