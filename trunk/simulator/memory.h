#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

typedef struct{
  uint32_t *base;
  unsigned int size;
  uint8_t *fp;
  uint8_t *sp;
}PROC_STACK;

typedef struct{

}PROC_MEM;

#endif
