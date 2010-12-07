#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>

#include "process.h"

int stack_initial(PROC_SEGMENT* stack, unsigned int stack_size){
  stack->vaddr_offset = 0x10000000 - stack_size;
  stack->size = stack_size;
  stack->flag = SEG_RD | SEG_WR;
  stack->base = malloc(stack_size * sizeof(uint8_t));
  return 1;
}

PROCESS* proc_initial(char* filename){
  PROCESS* proc;
  GElf_Phdr* phdr;
  int seg_num = 0;
  int i;
  FILE* fp;
  if((fp = fopen(filename, "r"))==NULL){
    fprintf(stderr, "File open error\n");
    exit(1);
  }

  proc = malloc(sizeof(PROCESS));
  proc->stack = NULL;
  proc->mem = NULL;
  ELF_initial(filename);
  seg_num = ELF_loadable_seg_num();
  proc->mem = mem_initial(seg_num+1);
  
  //Initial stack, the physical place of stack is proc->mem->segments[0]
  stack_initial(&(proc->mem->segments[0]), STACK_SIZE);
  proc->stack = &(proc->mem->segments[0]);
  
  for(i=1; i<=seg_num; i++){
    phdr = ELF_next_loadable_phdr();
    uint8_t* data = malloc(phdr->p_filesz + 1);
    fseek(fp, phdr->p_offset, 0);
    fread(data, phdr->p_filesz, 1, fp);
    segment_load(proc->mem, i, phdr->p_flags, phdr->p_vaddr,
    		 phdr->p_memsz, (void *)data, phdr->p_filesz);
  }

  proc->bp_list = bp_initial();

  proc->status = PROC_READY;
  //proc->entry = ELF_entry_point();
  proc->entry = ELF_main_entry();
  proc->list_cur_addr = proc->entry;
  //build symbal table
  ELF_build_symtbl(&(proc->symtbl));
  /*
  for(i=0; i<(proc->symtbl).sym_num; i++)
    printf("%d  %s  :  %d  :  0x%.8x\n", i, (proc->symtbl).name[i], ELF32_ST_TYPE((proc->symtbl).st_info[i]), (proc->symtbl).addr[i]);
  */
  ELF_close();
  return proc;
}

int proc_destroy(PROCESS* proc){
  int i;
  for(i=0; i<(proc->symtbl).sym_num; i++)
    free((proc->symtbl).name[i]);
  free((proc->symtbl).name);
  free((proc->symtbl).addr);
  free((proc->symtbl).st_info);
  proc->status = PROC_DEAD;
  if(proc->mem != NULL)
    mem_destroy(proc->mem);
  free(proc);
  bp_destroy(proc->bp_list);
  return 0;
}
