#ifndef BASIC_BLOCK_H_
#define BASIC_BLOCK_H_
#include "symtbl.h"

typedef struct basic_block {
	int begin;
	int end;
	struct basic_block* prev;
	struct basic_block* next;
	struct basic_block* jump;
}basic_block;

typedef struct func_block {
	basic_block* start;
	basic_block* over;
	struct func_block* prev;
	struct func_block* next;
}func_block;

#endif
