#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "CPU.h"

int console_next_cmd(CPU_d* cpu, char* filename);
CPU_d* process_start(char* filename);
void process_end(CPU_d* cpu);
// mode - 0 : normal mode
//        1 : trap mode
int console_run(CPU_d* cpu, char* filename, int mode);

#endif
