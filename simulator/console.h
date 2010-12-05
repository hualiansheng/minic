#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "CPU.h"

extern int console_next_cmd(CPU_d* cpu, char* filename);
extern CPU_d* process_start(char* filename);
extern void process_end(CPU_d* cpu);

#endif
