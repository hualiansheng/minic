#include "breakpoint.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

BP_LIST* bp_initial(){
  BP_LIST* bp_list;
  bp_list = malloc(sizeof(BP_LIST));
  bp_list->capacity = BP_INIT_CAPACITY;
  bp_list->num = 0;
  bp_list->count = 1;
  bp_list->data = malloc(BP_INIT_CAPACITY * sizeof(BP));
  memset(bp_list->data, 0, BP_INIT_CAPACITY * sizeof(BP));
  return bp_list;
}

void bp_destroy(BP_LIST* bp_list){
  free(bp_list->data);
  free(bp_list);
}

void bp_add(BP_LIST* bp_list, uint32_t addr){
  if(bp_list->num == bp_list->capacity){
    BP* tmp_data = malloc(2 * bp_list->capacity * sizeof(BP));
    memcpy(tmp_data, bp_list->data, bp_list->capacity * sizeof(BP));
    free(bp_list->data);
    bp_list->data = tmp_data;
    bp_list->capacity = bp_list->capacity * 2;
  }
  int pos, i;
  for(i = 0; i<bp_list->num; i++)
    if((bp_list->data[i]).bp_id == 0){
      pos = i;
      break;
    }
  (bp_list->data[bp_list->num]).bp_id = bp_list->count;
  (bp_list->data[bp_list->num]).addr = addr;
  (bp_list->data[bp_list->num]).visit_times = 0;
  bp_list->num ++;
  bp_list->count ++;
}

int bp_del(BP_LIST* bp_list, int bp_id){
  int i;
  for(i = 0; i < bp_list->capacity; i++)
    if((bp_list->data[i]).bp_id == bp_id){
      (bp_list->data[i]).bp_id = 0;
      bp_list->num --;
      return 1;
    }
  return 0;
}

void bp_show(BP_LIST* bp_list){
  int i;
  if(bp_list->num == 0){
    printf("No breakpoint.\n");
    return;
  }
  printf("Breakpoint list:==================\n");
  for(i = 0; i < bp_list->capacity; i++)
    if((bp_list->data[i]).bp_id != 0)
      printf("Breadpoint %d - address : 0x%.8x\n",
	     (bp_list->data[i]).bp_id,
	     (bp_list->data[i]).addr);
  printf("==================================\n");
}

// Judge if addr has a breakpoint
int bp_search(BP_LIST* bp_list, uint32_t addr){
  int i;
  for(i = 0; i < bp_list->capacity; i++)
    if((bp_list->data[i]).addr == addr){
      (bp_list->data[i]).visit_times ++;
      return (bp_list->data[i]).visit_times;
    }
  return 0;
}
