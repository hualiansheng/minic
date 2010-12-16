#include "CPU.h"
#include <malloc.h>
#include <memory.h>

#include "debugger.h"

CPU_d* CPU_initial(int id, int mode){
    CPU_d* cpu = malloc(sizeof(CPU_d));
    memset(cpu, 0, sizeof(CPU_d));
    cpu->cpu_id = id;
    cpu->mode = mode;
    cpu->regs = reg_initial();
    cpu->i_cache = cache_initial(NULL);
    cpu->d_cache = cache_initial(NULL);
    cpu->pipline = pipline_initial(cpu->regs, cpu->i_cache,
                                   cpu->d_cache);
    cpu->cpu_info = malloc(sizeof(CPU_info));
    memset(cpu->cpu_info, 0, sizeof(CPU_info));
    return cpu;
}

int CPU_destroy(CPU_d* cpu){
    reg_destroy(cpu->regs);
    cache_destroy(cpu->i_cache);
    cache_destroy(cpu->d_cache);
    pipline_destroy(cpu->pipline);
    free(cpu->cpu_info);
    return 1;
}

int CPU_load_process(CPU_d* cpu, PROCESS* proc){
    cpu->proc = proc;
    cache_mem_link(cpu->i_cache, proc->mem);
    cache_mem_link(cpu->d_cache, proc->mem);
    cpu->regs->REG_PC = proc->entry;
    cpu->regs->REG_SP = (uint32_t)proc->stack->vaddr_offset + (uint32_t)(proc->stack->size);
    cpu->pipline->stack = proc->stack;
    //printf("0x%.8x\n", (uint32_t)proc);
    cpu->pipline->proc = proc;
    return 1;
}

int CPU_next_beat(CPU_d* cpu){
    int i;
    //cpu->regs->r[24] = 1;
    for(i=0; i<20; i++){
        pipline_next_step(cpu->pipline, cpu->cpu_info);
        //debugger_print_mem(cpu->proc->mem, 0x020003cc);
        //debugger_modify_register(cpu->regs, 0, -1);
        //debugger_modify_mem(cpu->proc->mem, 0x020085b0, -1);
        //debugger_print_mem(cpu->proc->mem, 0x020085b0);
        debugger_print_stack(cpu->proc->mem, cpu->regs->REG_SP);
        debugger_modify_stack(cpu->proc->mem, cpu->regs->REG_SP,
                              cpu->regs->REG_SP, -1);
        printf("\n");
    }
    //debugger_print_cpu_info(cpu->cpu_info);
  
    //pipline_next_step(cpu->pipline, cpu->cpu_info);
    return 1;
}

int CPU_run(CPU_d* cpu, int cycles){

    return 0;
}

