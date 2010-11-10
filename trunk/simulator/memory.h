#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

typedef struct{
  uint8_t *base;
  unsigned int size;
  unsigned int used_size;
  uint8_t *sp;
}PROC_STACK;

typedef struct{
  unsigned int vaddr_offset;
  unsigned int size;
  uint8_t *base;
}PROC_SEGMENT;

typedef struct{
  unsigned int seg_num;
  PROC_SEGMENT * segments;
}PROC_MEM;

extern PROC_STACK* stack_initial(unsigned int stack_size);
extern void stack_destroy(PROC_STACK* stack);
extern int stack_push(PROC_STACK* stack, void *addr, unsigned int size);
extern int stack_pop(PROC_STACK* stack, void *addr, unsigned int size);

extern PROC_MEM* mem_initial(unsigned int _seg_num);
extern void mem_destroy(PROC_MEM *mem);
extern int segment_load(PROC_MEM* mem, unsigned int seg_index,
		 unsigned int vaddr_offset, unsigned int seg_size,
		 void* data, unsigned int data_size);
extern int mem_fetch(PROC_MEM* mem, unsigned int addr,
	      void *data, unsigned int data_size);
extern int mem_set(PROC_MEM* mem, unsigned int addr,
	    void *data, unsigned int data_size);

#endif
