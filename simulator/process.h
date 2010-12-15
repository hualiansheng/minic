#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "mem.h"
#include "breakpoint.h"
#include "ELF_parser.h"
#include <stdint.h>

#define	PROC_READY	0
#define	PROC_RUN	1
#define PROC_TRAP	2
#define PROC_DEAD	3
#define PROC_INITIAL	4
#define STACK_SIZE	4*1024*1024

typedef struct{
  int status;
  uint32_t entry;
  PROC_STACK* stack;
  PROC_MEM* mem;
  PROC_SYMTBL symtbl;
  BP_LIST* bp_list;
  uint32_t list_cur_addr;
  int step;
}PROCESS;

extern PROCESS* proc_initial(char* filename);
extern int proc_destroy(PROCESS* proc);

#endif
