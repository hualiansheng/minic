#ifndef _BREAKPOINT_H_
#define _BREAKPOINT_H_

#include <stdint.h>

#define BP_INIT_CAPACITY 20

typedef struct{
  uint32_t addr;
  int bp_id;
  int visit_times;
}BP;

typedef struct{
  int capacity;// Now capacity of breakpoint list
  int num;// Now numbers of breakpoints
  int count;// Now ID of breakpoints
  BP *data;
}BP_LIST;

extern BP_LIST* bp_initial();
extern void bp_destroy(BP_LIST* bp_list);
extern void bp_add(BP_LIST* bp_list, uint32_t addr);
extern void bp_del(BP_LIST* bp_list, int bp_id);
extern void bp_show(BP_LIST* bp_list);
extern int bp_search(BP_LIST* bp_list, uint32_t addr);

#endif
