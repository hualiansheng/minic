#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "mem.h"

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
  int i;
  for(i=0; i<mem->seg_num; i++)
    free((mem->segments[i]).base);
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
      //printf("Segment Num : %d\n", i);
      //printf("vaddr_offset : 0x%.8x, size: %d\n", (mem->segments[i]).vaddr_offset, (mem->segments[0]).size);
      //printf("High bound : 0x%.8x\n", (mem->segments[i]).vaddr_offset+ (mem->segments[i]).size);
      return 0;
    }
  }
  printf("vaddr_offset : 0x%.8x, size: %d\n", (mem->segments[i]).vaddr_offset, (mem->segments[0]).size);
  printf("High bound : 0x%.8x\n", (mem->segments[i]).vaddr_offset+ (mem->segments[i]).size);
  data = NULL;
  fprintf(stderr, "mem_fetch : Not an effective address : 0x%.8x\n", addr);
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
       (mem->segments[i]).size * 4){
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
       (mem->segments[i]).size * 4){
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
       (mem->segments[i]).size*4)
      return 0;
  return -1;
}

