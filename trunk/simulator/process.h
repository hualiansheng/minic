#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "memory.h"

#define	PROC_READY	0
#define	PROC_RUN	1
#define PROC_TRAP	2
#define PROC_DEAD	3
#define STACK_SIZE	4096

typedef struct{
  int status;
  PROC_STACK* stack;
  PROC_MEM* mem;
}PROCESS;

extern PROCESS* proc_load(char* filename);
extern int proc_destroy(PROCESS* proc);

#endif