#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include "CPU.h"
#include <stdint.h>

#define BYTE	0
#define HWORD 1
#define WORD	2

void debugger_print_cpu_info(CPU_info* cpu_info);
void debugger_print_register(REGISTERS* regs, int reg_num);
void debugger_print_CMSR(REGISTERS* regs);
void debugger_modify_register(REGISTERS* regs, int reg_num, int32_t content);
void debugger_print_mem(PROC_MEM* mem, uint32_t addr);
void debugger_modify_mem(PROC_MEM* mem, uint32_t addr, int32_t content);
void debugger_print_stack(PROC_MEM* mem, uint32_t sp);
void debugger_modify_stack(PROC_MEM* mem, uint32_t sp,
                           uint32_t addr, int32_t content);
int debugger_search_symtbl(PROCESS* proc, uint32_t addr, char* result,
                           int type);
uint32_t debugger_search_symtbl_func(PROCESS* proc, char* func_name);
void debugger_print_inst_statistic(PROCESS* proc);

#endif
