#ifndef _ATTR_H_
#define _ATTR_H_
#include "AST.h"
typedef struct iattr{
	int ival;
	AST_NODE* ptr;	
}iattr;
typedef struct cattr{
	char cval;
	AST_NODE* ptr;
}cattr;
typedef struct sattr{
	char* sval;
	AST_NODE* ptr;
}sattr;
#endif
