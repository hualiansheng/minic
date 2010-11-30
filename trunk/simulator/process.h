#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "mem.h"
#include <stdint.h>

#define	PROC_READY	0
#define	PROC_RUN	1
#define PROC_TRAP	2
#define PROC_DEAD	3
#define PROC_INITIAL	4
#define STACK_SIZE	8192

typedef struct{
  int status;
  uint32_t entry;
  PROC_STACK* stack;
  PROC_MEM* mem;
}PROCESS;

extern PROCESS* proc_initial(char* filename);
extern int proc_destroy(PROCESS* proc);

#endif
