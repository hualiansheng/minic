#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>

#define	SEG_RD	4
#define	SEG_WR	2
#define SEG_EX	1

#define DATA_RD 4
#define DATA_WR 2
#define DATA_EX 1

typedef struct{
  unsigned int vaddr_offset;
  unsigned int size;
  uint8_t *base;
  int flag;
}PROC_SEGMENT;

typedef struct{
  unsigned int seg_num;
  PROC_SEGMENT * segments;
}PROC_MEM;

typedef PROC_SEGMENT PROC_STACK;

extern PROC_MEM* mem_initial(unsigned int _seg_num);
extern void mem_destroy(PROC_MEM *mem);
extern int segment_load(PROC_MEM* mem, unsigned int seg_index, int flag,
		 unsigned int vaddr_offset, unsigned int seg_size,
		 void* data, unsigned int data_size);
extern int mem_fetch(PROC_MEM* mem, uint32_t addr,
		     void *data, unsigned int data_size,
		     int data_type);
extern int mem_set(PROC_MEM* mem, uint32_t addr,
	    void *data, unsigned int data_size);
extern int mem_type(PROC_MEM* mem, uint32_t addr);
extern int mem_invalid(PROC_MEM* mem, uint32_t addr);// incalid return 0, else return -1

#endif
