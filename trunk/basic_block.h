#include "symtbl.h"

typedef struct basic_block {
	int begin;
	int end;
	basic_block* prev;
	basic_block* next;
	basic_block* jump;
}basic_block;

typedef struct func_block {
	basic_block* start;
	basic_block* over;
	func_block* prev;
	func_block* next;
}func_block;
