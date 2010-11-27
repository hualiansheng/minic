#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "memory.h"

//Initialize stack, the stack_size is based in Byte
PROC_STACK * stack_initial(unsigned int stack_size){
  PROC_STACK *stack = malloc(sizeof(PROC_STACK));
  //printf("stack initial addr: %x\n", (unsigned int)stack);
  stack->size = stack_size;
  stack->base = malloc(stack_size);
  stack->used_size = 0;
  stack->sp = stack->base;
  return stack;
}

//destroy stack
void stack_destroy(PROC_STACK * stack){
  free(stack->base);
  free(stack);
}

//allocate for a stack, based in byte
int stack_allocate(PROC_STACK* stack, int size){
  if(stack->used_size + size <= stack->size){
    stack->sp = stack->sp + size;
    stack->used_size = stack->used_size + size;
    return size;
  }else{
    int tmp = stack->size - stack->used_size;
    stack->used_size = stack->size;
    stack->sp += tmp;
    return tmp;
  }
}

//free for a stack, based in byte
int stack_free(PROC_STACK* stack, int size){
  if(stack->used_size >= size){
    stack->used_size -= size;
    stack->sp -= size;
    return size;
  }else{
    int tmp = stack->used_size;
    stack->used_size = 0;
    stack->sp = stack->base;
    return tmp;
  }
}

//test if addr is the address in stack
//true returns 1, false returns 0
int stack_test_addr(PROC_STACK* stack, uint32_t addr){
  if(addr >= (uint32_t)stack->base
     && addr< (uint32_t)(stack->base + stack->size))
    return 1;
  return 0;
}

/*
//push onto stack
//addr is the address of the data
//size is the size of data, based on Byte
int stack_push(PROC_STACK * stack, void * addr, unsigned int size){
  if(stack->used_size + size > stack->size){
    fprintf(stderr, "Stack high overflow.\n");
    exit(1);
  }
  memcpy(stack->sp, addr, size);
  stack->used_size = stack->used_size + size;
  stack->sp = stack->sp + size;
  return 0;
}

//pop from stack
//addr is the address of the data
//size is the size of data, based on Byte
int stack_pop(PROC_STACK *stack, void* addr, unsigned int size){
  if(stack->used_size < size){
    fprintf(stderr, "Stack low overflow.\n");
    exit(1);
  }
  stack->sp = stack->sp - size;
  memcpy(addr, stack->sp, size);
  return 0;
}
*/

//Above is stack implementation
//---------------------------------------------------------------------

//Initial memory
PROC_MEM* mem_initial(unsigned int _seg_num){
  PROC_MEM *mem;
  mem = malloc(sizeof(PROC_MEM));
  mem->seg_num = _seg_num;
  mem->segments = malloc(_seg_num * sizeof(PROC_SEGMENT));
  return mem;
}

//Destroy memory
void mem_destroy(PROC_MEM* mem){
  free(mem->segments);
  free(mem);
}

//Load segment
//size is the size of the segment, based on Byte
//vaddr_offset is the offset of virtual address
int segment_load(PROC_MEM* mem, unsigned int seg_index, int flag,
		 unsigned int vaddr_offset, unsigned int seg_size,
		 void* data, unsigned int data_size){
  if(mem == NULL){
    fprintf(stderr, "Uninitialization of memory.\n");
    exit(1);
  }
  if(seg_index >= mem->seg_num){
    fprintf(stderr, "seg_index is out of range.\n");
    exit(1);
  }
  (mem->segments[seg_index]).flag = flag;
  (mem->segments[seg_index]).size = seg_size;
  (mem->segments[seg_index]).vaddr_offset = vaddr_offset;
  (mem->segments[seg_index]).base = malloc(seg_size);
  memcpy((mem->segments[seg_index]).base, data, data_size);
  return 0;
}

//Fetch data from mem
//data_type is defined in memory.h
int mem_fetch(PROC_MEM* mem, unsigned int addr,
	      void *data, unsigned int data_size,
	      int data_type){
  if(mem == NULL){
    fprintf(stderr, "Uninitialization of memory.\n");
    exit(1);
  }
  int i;
  for(i=0; i<mem->seg_num; i++){
    //printf("segment offset : %x\n", (mem->segments[i]).vaddr_offset);
    //printf("segment flag : %x\n", (mem->segments[i]).vaddr_offset);
    //printf("%x\n",addr);
    if(((mem->segments[i]).flag & data_type) != 0
       && (mem->segments[i]).vaddr_offset <= addr
       && addr <= (mem->segments[i]).vaddr_offset +
       (mem->segments[i]).size){
      //printf("segment %d : size=%d\n", i, (mem->segments[i]).size);
      uint8_t *src_addr = (mem->segments[i]).base;
      unsigned int offset;
      offset = (addr-(unsigned int)(mem->segments[i]).vaddr_offset);
      //printf("offset=%d\n", offset);
      src_addr = src_addr + offset;
      memcpy(data, src_addr, data_size);
      return 0;
    }
  }
  data = NULL;
  fprintf(stderr, "mem_fetch : Not an effective address\n");
  exit(1);
}

//Set data from mem
int mem_set(PROC_MEM* mem, unsigned int addr,
	    void *data, unsigned int data_size){
  if(mem == NULL){
    fprintf(stderr, "Uninitialization of memory.\n");
    exit(1);
  }
  int i;
  for(i=0; i<mem->seg_num; i++)
    if(((mem->segments[i]).flag & SEG_WR) !=0
       && (mem->segments[i]).vaddr_offset <= addr
       && addr <= (mem->segments[i]).vaddr_offset + 
       (mem->segments[i]).size){
      uint8_t *dest_addr = (mem->segments[i]).base;
      unsigned int offset;
      offset = (addr-(unsigned int)(mem->segments[i]).vaddr_offset);
      dest_addr = dest_addr + offset;
      memcpy(dest_addr, data, data_size);
      return 0;
    }
  return 2;
}

int mem_type(PROC_MEM* mem, uint32_t addr){
  if(mem == NULL){
    fprintf(stderr, "Uninitialization of memory.\n");
    exit(1);
  }
  int i;
  for(i=0; i<mem->seg_num; i++)
    if((mem->segments[i]).vaddr_offset <= addr
       && addr <= (mem->segments[i]).vaddr_offset +
       (mem->segments[i]).size){
      return (mem->segments[i]).flag;
    }
  fprintf(stderr, "mem_type : Not an effective address\n");
  exit(1);
}

int mem_invalid(PROC_MEM* mem, uint32_t addr){
  if(mem == NULL){
    fprintf(stderr, "Uninitialization of memory.\n");
    exit(1);
  }
  int i;
  for(i=0; i<mem->seg_num; i++)
    if((mem->segments[i]).vaddr_offset <= addr
       && addr <= (mem->segments[i]).vaddr_offset +
       (mem->segments[i]).size)
      return 0;
  return -1;
}
