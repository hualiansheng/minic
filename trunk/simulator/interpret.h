#ifndef _INTERPRET_H_
#define _INTERPRET_H_

#include "process.h"

int interpret_inst(uint32_t inst_code, 
                   uint32_t inst_addr, char* result, PROCESS* proc);

#endif
