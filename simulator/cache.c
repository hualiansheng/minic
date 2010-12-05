#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include "cache.h"

CACHE* cache_initial(PROC_MEM* _mem){
  CACHE* cache = malloc(sizeof(CACHE));
  memset(cache, 0, sizeof(CACHE));
  cache->block_num = CACHE_SIZE/CACHE_BLOCK_SIZE;
  cache->sign_bits_num = WORD_WIDTH - (int)(log(CACHE_SIZE)/log(2));
  cache->mem = _mem;
  //printf("cache sign bits num : %d\n", cache->sign_bits_num);
  /*
  uint32_t i = 0x20085b0;
  for(;i<0x2008670; i+=4)
    cache_search(cache, i);
  */
  return cache;
}

CACHE_RETURN cache_miss(CACHE* cache, uint32_t addr){
  CACHE_RETURN c_r;
  uint32_t block_addr;
  int block_offset;
  int block_index;
  int mark_addr;
  int in_block_index;
  uint32_t *presult;
  int in_block_count = 0;
  block_index = (addr % CACHE_SIZE)/CACHE_BLOCK_SIZE;
  mark_addr = addr >> ((int)(log(CACHE_SIZE)/log(2)));
  in_block_index = addr % CACHE_BLOCK_SIZE;
  block_offset = (int)(log(CACHE_BLOCK_SIZE)/log(2));
  block_addr = (addr >> block_offset) << (block_offset);

  c_r.cpu_cycles = -1;
  cache->mark[block_index] = mark_addr;
  while(mem_invalid(cache->mem, block_addr) != 0){
    block_addr += 4;
    in_block_count += 4;
  }
  if(in_block_count > 32){
    fprintf(stderr, "Address invalid.\n");
    exit(1);
  }
  else if(mem_fetch(cache->mem, block_addr,
		    &(cache->data)[block_index][in_block_count],
		    CACHE_BLOCK_SIZE-in_block_count, DATA_RD) != 0){
    return c_r;
  }
  c_r.cpu_cycles = CACHE_MISSED_CYCLE;
  presult = (uint32_t*)&(cache->data[block_index][in_block_index]);
  c_r.data = *presult;
  //printf("Cache missed! Data : 0x%x\n", c_r.data);
  return c_r;
}

CACHE_RETURN cache_hit(CACHE* cache, uint32_t addr){
  CACHE_RETURN c_r;
  int block_index;
  int mark_addr;
  int in_block_index;
  uint32_t *presult;
  block_index = (addr % CACHE_SIZE)/CACHE_BLOCK_SIZE;
  mark_addr = addr >> ((int)(log(CACHE_SIZE)/log(2)));
  in_block_index = addr % CACHE_BLOCK_SIZE;

  cache->mark[block_index] = mark_addr;
  c_r.cpu_cycles = CACHE_HIT_CYCLE;
  presult = (uint32_t*)&(cache->data[block_index][in_block_index]);
  c_r.data = *presult;
  //printf("Cache Hit! Data : 0x%x\n", c_r.data);
  return c_r;
}

int cache_rewrite(CACHE* cache, uint32_t addr){
  int block_index;
  block_index = (addr % CACHE_SIZE)/CACHE_BLOCK_SIZE;
  int type = mem_type(cache->mem, addr);
  if((type & SEG_WR) != 0){
    uint32_t block_addr;
    int block_offset;
    int mark_addr;
    int in_block_index;
    int in_block_count = 0;
    mark_addr = addr >> ((int)(log(CACHE_SIZE)/log(2)));
    in_block_index = addr % CACHE_BLOCK_SIZE;
    block_offset = (int)(log(CACHE_BLOCK_SIZE)/log(2));
    block_addr = (mark_addr << block_offset)
      + block_index * CACHE_BLOCK_SIZE;

    while(mem_invalid(cache->mem, block_addr) != 0){
      block_addr += 4;
      in_block_count += 4;
    }
    mem_set(cache->mem, block_addr, (cache->data)[block_index],
	    CACHE_BLOCK_SIZE-in_block_count);
    //printf("Rewrite NO. %d block.\n", block_index);
    return 0;
  }
  else if(type != -1){
    fprintf(stderr,
	    "Warning : Read only address, NO. %d block rewrite ignored.\n",
	    block_index);
    return 1;
  }
  return -1;
}

CACHE_RETURN cache_search(CACHE* cache, uint32_t addr){
  CACHE_RETURN c_r;
  memset(&c_r, 0, sizeof(CACHE_RETURN));
  int block_index;
  int mark_addr;
  int in_block_index;
  block_index = (addr % CACHE_SIZE)/CACHE_BLOCK_SIZE;
  mark_addr = addr >> ((int)(log(CACHE_SIZE)/log(2)));
  in_block_index = addr % CACHE_BLOCK_SIZE;
  //printf("block index : %x\n", block_index);
  //printf("mark_addr : %x\n", mark_addr);
  //printf("in block index : %x\n", in_block_index);
  if(cache->mem == NULL){
    fprintf(stderr, "No memory linked to cache!");
    exit(1);
  }
  if(cache->valid[block_index] == 0){
    cache->valid[block_index] = 1;
    return cache_miss(cache, addr);
  }
  else if(cache->mark[block_index] == mark_addr){
    return cache_hit(cache, addr);
  }
  else{
    int re_tmp = cache_rewrite(cache, addr);
    if(re_tmp == 0)
      return cache_miss(cache, addr);
    else
      c_r.cpu_cycles = -1;
  }
  return c_r;
}

int cache_write(CACHE* cache, uint32_t addr, uint32_t data, int data_type){
  CACHE_RETURN c_r;
  memset(&c_r, 0, sizeof(CACHE_RETURN));
  int block_index;
  int mark_addr;
  int in_block_index;
  block_index = (addr % CACHE_SIZE)/CACHE_BLOCK_SIZE;
  mark_addr = addr >> ((int)(log(CACHE_SIZE)/log(2)));
  in_block_index = addr % CACHE_BLOCK_SIZE;
  //printf("block index : %x\n", block_index);
  //printf("mark_addr : %x\n", mark_addr);
  //printf("in block index : %x\n", in_block_index);
  if(cache->mem == NULL){
    fprintf(stderr, "No memory linked to cache!");
    exit(1);
  }
  if(cache->valid[block_index] == 0){
    cache->valid[block_index] = 1;
    c_r = cache_miss(cache, addr);
  }
  else if(cache->mark[block_index] == mark_addr){
    c_r = cache_hit(cache, addr);
  }
  else{
    int re_tmp = cache_rewrite(cache, addr);
    if(re_tmp == 0)
      c_r = cache_miss(cache, addr);
    else
      c_r.cpu_cycles = -1;
  }
  // Make data right
  if(data_type == DATA_B){
    data = data & 0x000000FF;
    data = data | (c_r.data & 0xFFFFFF00);
  }else if(data_type == DATA_HW){
    data = data & 0x0000FFFF;
    data = data | (c_r.data & 0xFFFF0000);
  }
  int32_t* presult;
  presult = (int32_t*)&(cache->data[block_index][in_block_index]);
  *presult = data;
  return 0;
}

int cache_destroy(CACHE* cache){
  free(cache);
  return 1;
}

int cache_mem_link(CACHE* cache, PROC_MEM* _mem){
  cache->mem = _mem;
  return 1;
}
