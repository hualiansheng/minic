#ifndef _ATTR_H_
#define _ATTR_H_
#include "AST.h"
struct iattr{
	int ival;
	AST_NODE* ptr;	
};
struct cattr{
	char cval;
	AST_NODE* ptr;
};
struct sattr{
	char* sval;
	AST_NODE* ptr;
};
