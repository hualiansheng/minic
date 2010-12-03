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

#define EQ    0
#define NE    1
#define UGE   2
#define ULT   3
#define N     4
#define NN    5
#define OV    6
#define NV    7
#define UGT   8
#define ULE   9
#define SGE   10
#define SLT   11
#define SGT   12
#define SLE   13
#define AL    14

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
