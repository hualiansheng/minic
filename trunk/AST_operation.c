#include "AST.h"
#include <stdio.h>

AST_NODE* AST_new_node(){
  	AST_NODE* result = (AST_NODE *)malloc(sizeof(AST_NODE));
  	result->father = NULL;
  	result->leftChild = NULL;
  	return result;
}

void AST_addChild(AST_NODE* root, int child_nodeType, AST_NODE* child){
	AST_NODE* p = root -> leftChild;
	if(p != NULL) root -> leftChild = child;
	else{
		while(p -> rightSibling != NULL){
			p = p -> rightSibling;
		}
		p -> rightSibling = child;
	}
	child -> nodeLevel = root -> nodeLevel + 1;
	child -> nodeType = child_nodeType;
}
