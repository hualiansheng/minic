#ifndef _SYMTBL_H
#define _SYMTBL_H
char* name_pool;
int name_off;
int name_size;
typedef struct symtbl_hdr symtbl_hdr;
typedef struct symtbl_item symtbl_item;
typedef struct symtbl_func_para symtbl_func_para;

struct symtbl_hdr
{
	symtbl_hdr* parent_tbl;
	int ret_type;
	int ret_star;
	int para_num;
	int item_num;
	int maxSize;
	symtbl_item* item;
};
struct symtbl_item
{
	int type;
	int star_num;
	int writable;
	char* name;
	int size;
};
/*struct symtbl_func_para
{
	int type;
	int star_num;
	int writable;
	char* name;
	int size;
	symtbl_func_para* next;
};*/
#endif
