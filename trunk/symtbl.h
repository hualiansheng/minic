#ifndef _SYMTBL_H
#define _SYMTBL_H
char* name_pool;
typedef struct symtbl_hdr
{
	symtbl_hdr* parent_tbl;
	int item_num;
	int maxSize;
	symtbl_item* item;
} symtbl_hdr;
typedef struct symtbl_item
{
	int type;
	int star_num;
	int writable;
	char* name;
	int size;
	int ret_type;
	symtbl_func_para* paralist;
} symtbl_item;
typedef struct symtbl_func_para
{
	int type;
	int star_num;
	int writable;
	char* name;
	int size;
	symtbl_func_para* next;
} symtbl_func_para;
#endif
