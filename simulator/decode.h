#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include "pipline.h"

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

extern int inst_decode(PIPLINE_DATA* _pipline_data);

#endif
