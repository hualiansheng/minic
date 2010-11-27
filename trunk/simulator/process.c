#include <malloc.h>
#include <stdlib.h>

#include "ELF_parser.h"
#include "process.h"

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
  //printf("loadable segment num : %d\n", seg_num);
  proc->stack = stack_initial(STACK_SIZE);
  proc->mem = mem_initial(seg_num);
  
  for(i=0; i<seg_num; i++){
    phdr = ELF_next_loadable_phdr();
    uint8_t* data = malloc(phdr->p_filesz + 1);
    fseek(fp, phdr->p_offset, 0);
    fread(data, phdr->p_filesz, 1, fp);
    segment_load(proc->mem, i, phdr->p_flags, phdr->p_vaddr,
    		 phdr->p_memsz, (void *)data, phdr->p_filesz);
  }

  proc->status = PROC_READY;
  //proc->entry = ELF_entry_point();
  proc->entry = ELF_main_entry();

  ELF_close();
  /*
  uint8_t d;
  for(i=0x020085b0; i<0x020086c0; i++){
    if(mem_fetch(proc->mem, i, &d, sizeof(d), DATA_RD) == 0){
      printf("addr=%x\tvalue=%x\n", i, d);
    }
  }
  for(i=0; i<0x110; i++){
    printf("addr=%x\tvalue=%x\n", i, proc->mem->segments[1].base[i]);
    }
  */
  //printf("%x\n", proc->mem->segments[1].size);
  return proc;
}

int proc_destroy(PROCESS* proc){
  proc->status = PROC_DEAD;
  if(proc->mem != NULL)
    mem_destroy(proc->mem);
  if(proc->stack != NULL)
    stack_destroy(proc->stack);
  free(proc);
  return 0;
}
