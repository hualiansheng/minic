#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

#include "CPU.h"
#include "memory.h"
#include <stdint.h>

#define BYTE	0
#define WORD	1
#define DWORD	2

extern void print_cpu_info(CPU_info* cpu_info);
extern void print_mem(PROC_MEM* mem, uint32_t addr, int data_type);

#endif
