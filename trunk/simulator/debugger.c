#include "debugger.h"
#include <string.h>

// print cpu infomation
// cpu_info - the struct of cpu information
void debugger_print_cpu_info(CPU_info* cpu_info){
  printf("CPU info:===========================\n");
  printf("\tCPU total cycle : %d\n", cpu_info->cycles_total);
  printf("\tCPU idle cycle : %d\n", cpu_info->cycles_total - cpu_info->cycles_work);
  printf("\tPipline bubbles : %d\n", cpu_info->bubbles);
  printf("\tRead memory times : %d\n", cpu_info->rd_mem_times);
  printf("\tWrite memory times : %d\n", cpu_info->wr_mem_times);
  printf("\tCache visit times : %d\n", cpu_info->cache_visit);
  printf("\tCache miss times : %d\n", cpu_info->cache_miss);
  printf("\t----------------\n");
  printf("\tCPU idle rate : %.2lf%%\n", (cpu_info->cycles_total - cpu_info->cycles_work) * 100.0 /cpu_info->cycles_total);
  printf("\tCache miss rate : %.2lf%%\n", (double)(cpu_info->cache_miss) / (double)(cpu_info->cache_visit));
  printf("====================================\n");
}

// print registers
// regs - the struct of register heap
// reg_num - -1 : print all registers
//           0~31 : print the exact register
void debugger_print_register(REGISTERS* regs, int reg_num){
  if(reg_num >= 0 && reg_num < 32){
    printf("Reg%d : 0x%.8x\n", reg_num, regs->r[reg_num]);
    return;
  }
  if(reg_num != -1){
    printf("Invalid register number : %d\n", reg_num);
    return;
  }
  int i;
  printf("Register Heap Info:=================\n");
  for(i=0; i<32; i++){
    if(i % 2 == 0 && i != 0)
      printf("\n");
    printf("Reg%d : 0x%.8x", i, regs->r[i]);
    if(i % 4 != 3)
      printf("\t");
  }
  printf("\n");
  printf("====================================\n");
}

void debugger_modify_register(REGISTERS* regs, int reg_num, int32_t content){
  if(reg_num < 0 || reg_num >= 32){
    printf("Invalid register number : %d\n", reg_num);
    return;
  }
  printf("Modify r[%d] : prev val - 0x%.8x  new val - 0x%.8x\n",
	 reg_num, regs->r[reg_num], content);
  regs->r[reg_num] = content;
}

// print memory of addr
// mem - the struct of memory
// addr - the address to print
void debugger_print_mem(PROC_MEM* mem, uint32_t addr){
  if(mem_invalid(mem, addr) != 0){
    printf("Invalid memory address : 0x%.8x.\n", addr);
    return;
  }
  uint32_t data;
  mem_fetch(mem, addr, &data, sizeof(data), DATA_RD);
  printf("Memory 0x%.8x : 0x%.8x\n", addr, data);
}

void debugger_modify_mem(PROC_MEM* mem, uint32_t addr, int32_t content){
  if(mem_invalid(mem, addr) != 0){
    printf("Invalid memory address : 0x%.8x.\n", addr);
    return;
  }
  if((mem_type(mem, addr) & SEG_WR) == 0){
    printf("Unwriteable memory address : 0x%.8x\n", addr);
    return;
  }
  int32_t data = content;
  mem_set(mem, addr, &data, sizeof(data));
  printf("Modify memory 0x%.8x : 0x%.8x\n", addr, content);
}

void debugger_print_stack(PROC_MEM* mem, uint32_t sp){
  uint32_t stack_top;
  uint32_t i;
  uint32_t data;
  stack_top = (mem->segments[0]).vaddr_offset + (mem->segments[0]).size;
  printf("stack info=========================\n");
  for(i = stack_top; i>= sp; i-=4){
    if(mem_invalid(mem, i) != 0){
      printf("Invalid stack address : 0x%.8x\n", i);
      printf("===================================\n");
      break;
    }
    mem_fetch(mem, i, &data, sizeof(data), DATA_RD);
    if(i == stack_top){
      if(i == sp)
	printf("0x%.8x : 0x%.8x <----Stack Top <--- SP\n", i, data);
      else
	printf("0x%.8x : 0x%.8x <----Stack Top\n", i, data);
    }
    else if(i == sp)
      printf("0x%.8x : 0x%.8x <----Sp\n", i, data);
    else
      printf("0x%.8x : 0x%.8x\n", i, data);
  }
  printf("===================================\n");
  //printf("Stack top addr : 0x%.8x\n", stack_top);
}

void debugger_modify_stack(PROC_MEM* mem, uint32_t sp,
			   uint32_t addr, int32_t content){
  uint32_t stack_top;
  stack_top = (mem->segments[0]).vaddr_offset + (mem->segments[0]).size;
  if(addr < sp || addr > stack_top){
    printf("Address 0x%.8x out of bounds from stack top to sp\n", addr);
    return;
  }
  printf("Modify stack 0x%.8x : 0x%.8x\n", addr, content);
  debugger_modify_mem(mem, addr, content);
}

// search the name of the given address in symbal table
// return val : 0 no such address
//              1 have such address
int debugger_search_symtbl(PROCESS* proc, uint32_t addr, char* result,
			   int type){
  //printf("0x%.8x\n", (uint32_t)proc);
  PROC_SYMTBL symtbl = proc->symtbl;
  int i;
  for(i=0; i<symtbl.sym_num; i++){
    if(symtbl.addr[i] == addr && 
       ELF32_ST_TYPE(symtbl.st_info[i]) == type){
      strcpy(result, symtbl.name[i]);
      return 1;
    }
  }
  return 0;
}

uint32_t debugger_search_symtbl_func(PROCESS* proc, char* func_name){
  PROC_SYMTBL symtbl = proc->symtbl;
  int i;
  for(i=0; i<symtbl.sym_num; i++){
    //printf("%d  %s 0x%.8x", i, symtbl.name[i], symtbl.addr[i]);
    if(strcmp(symtbl.name[i], func_name) == 0 && 
       ELF32_ST_TYPE(symtbl.st_info[i]) == STT_FUNC){
      return symtbl.addr[i];
    }
  }
  return 0;
}

void debugger_set_break_point(CPU_d* cpu, uint32_t addr){
  
}

void debugger_remove_break_point(CPU_d* cpu, uint32_t addr){

}

void debugger_list_break_point(CPU_d* cpu, uint32_t addr){

}

void debugger_list_codes(CPU_d* cpu, uint32_t addr, int lines){

}

