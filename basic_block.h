#ifndef BASIC_BLOCK_H_
#define BASIC_BLOCK_H_
#include "symtbl.h"
#include "gen_intermediate_code.h"


typedef struct basic_block {
	int begin;
	int end;
	int m;
	struct basic_block* prev;
	struct basic_block* next;
	struct basic_block* follow;
	struct basic_block* jump;
}basic_block;

typedef struct map_table {
	int isTmp;
	int tmp_k;
	char* var_name;
}map_table;

typedef struct func_block {
	basic_block* start;
	basic_block* over;
	struct func_block* prev;
	struct func_block* next;
	int code_num;
	int bb_num;
	int width;
	symtbl_item** uni_table;
	int uni_item_num;
	int uni_table_size;
	unsigned int **live_status;
	unsigned int **v_out;
	unsigned int **v_in;
	unsigned int **def;
	unsigned int **use;
	map_table* mapping;
	int map_table_size;
}func_block;

#endif
