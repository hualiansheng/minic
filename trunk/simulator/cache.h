#ifndef _CACHE_H_
#define _CACHE_H_

#include <stdint.h>
#include "mem.h"

//All macro is in Byte
#define CACHE_SIZE		1024
#define CACHE_BLOCK_SIZE	32
#define CACHE_NUM		1
#define WORD_WIDTH		32
#define CACHE_MISSED_CYCLE	8
#define CACHE_HIT_CYCLE		1

typedef struct{
  int block_num;
  int sign_bits_num;
  PROC_MEM* mem;
  int valid[CACHE_SIZE/CACHE_BLOCK_SIZE];
  uint8_t data[CACHE_SIZE/CACHE_BLOCK_SIZE][CACHE_BLOCK_SIZE];
  uint32_t mark[CACHE_SIZE/CACHE_BLOCK_SIZE];
}CACHE;

typedef struct{
  int cpu_cycles;
  uint32_t data;
}CACHE_RETURN;

extern CACHE* cache_initial(PROC_MEM* _mem);
extern int cache_mem_link(CACHE* cache, PROC_MEM* _mem);
extern CACHE_RETURN cache_search(CACHE* cache,uint32_t addr);
extern int cache_destroy(CACHE* cache);

#endif
