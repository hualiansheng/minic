#ifndef _INST_H_
#define _INST_H_

#define	AND	0
#define	XOR 	1
#define SUB 	2
#define RSB 	3
#define ADD 	4
#define ADC 	5
#define SBC 	6
#define RSC 	7
#define CAND 	8
#define CXOR 	9
#define CSUB 	10
#define CADD 	11
#define ORR 	12
#define MOV 	13
#define CLB 	14
#define MVN 	15

#define D_IMM_SHIFT    0
#define D_REG_SHIFT    1
#define MULTIPLY       2
#define BRANCH_EX      3
#define D_IMMEDIATE    4
#define L_S_R_OFFSET   5
#define L_S_HW_SB_ROF  6
#define L_S_HW_SB_IOF  7
#define L_S_I_OFFSET   8
#define BRANCH_LINK    9
#define ST             10

#endif
