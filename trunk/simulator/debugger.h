#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include "CPU.h"
#include "memory.h"
#include <stdint.h>

#define BYTE	0
#define WORD	1
#define DWORD	2

extern void debugger_print_cpu_info(CPU_info* cpu_info);
extern void debugger_print_mem(PROC_MEM* mem, uint32_t addr, int data_type);
extern void debugger_print_stack(PROC_STACK* stack);
extern void debugger_modify_mem(PROC_MEM* mem, uint32_t addr, int data_type);
extern void debugger_modify_stack(PROC_STACK* stack, uint32_t addr, int data_type);
extern void debugger_set_break_point(CPU_d* cpu, uint32_t addr);
extern void debugger_remove_break_point(CPU_d* cpu, uint32_t addr);
extern void debugger_list_break_point(CPU_d* cpu, uint32_t addr);
extern void debugger_list_codes(CPU_d* cpu, uint32_t addr, int lines);

#endif
