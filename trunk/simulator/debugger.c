#include "debugger.h"

void debugger_print_cpu_info(CPU_info* cpu_info){
  printf("CPU info:===========================\n");
  printf("\tCPU total cycle : %d\n", cpu_info->cycles_total);
  printf("\tCPU idle cycle : %d\n", cpu_info->cycles_total - cpu_info->cycles_work);
  printf("\tPipline bubbles : %d\n", cpu_info->bubbles);
  printf("\tRead memory times : %d\n", cpu_info->rd_mem_times);
  printf("\tWrite memory times : %d\n", cpu_info->wr_mem_times);
  printf("\tCache visit times : %d\n", cpu_info->cache_visit);
  printf("\tCache miss times : %d\n", cpu_info->cache_miss);
  printf("====================================\n");
}

void debugger_print_mem(PROC_MEM* mem, uint32_t addr, int data_type){
  
}
