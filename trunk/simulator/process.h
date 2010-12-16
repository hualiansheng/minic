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
  int inst_total;// total instruction number
  int inst_d_imm_shift;// d_imm_shift instruction number
  int inst_d_reg_shift;
  int inst_d_immediate;
  int inst_multiply;
  int inst_branch_ex;
  int inst_branch_link;
  int inst_l_s_r_offset;
  int inst_l_s_i_offset;
  int inst_l_s_hw_sb_rof;
  int inst_l_s_hw_sb_iof;
  int inst_st;
  int inst_unknown;
}INST_STATISTIC;

typedef struct{
  int status;
  uint32_t entry;
  PROC_STACK* stack;
  PROC_MEM* mem;
  PROC_SYMTBL symtbl;
  BP_LIST* bp_list;
  uint32_t list_cur_addr;
  int step;
  INST_STATISTIC inst_static;
}PROCESS;

extern PROCESS* proc_initial(char* filename);
extern int proc_destroy(PROCESS* proc);

#endif
