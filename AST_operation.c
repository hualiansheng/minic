#include "AST.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
//extern char name[][30];

AST_NODE* AST_new_Node(){
  	AST_NODE* result = (AST_NODE *)malloc(sizeof(AST_NODE));
  	result->father = NULL;
  	result->leftChild = NULL;
  	return result;
}

void AST_addChild(AST_NODE* root, int child_nodeType, AST_NODE* child){
	AST_NODE* p = root -> leftChild;
	if(p == NULL) root -> leftChild = child;
	else{
		while(p -> rightSibling != NULL){
			p = p -> rightSibling;
		}
		p -> rightSibling = child;
	}
	child -> nodeLevel = root -> nodeLevel + 1;
	child -> nodeType = child_nodeType;
	child -> father = root;
}
void tree_traversal(AST_NODE* root, int (*f[])(AST_NODE*))
{
	AST_NODE* p;
//	fprintf(stderr,"tree_traversal: %s\n",name[root->nodeType-FUNC_OFFSET]);
	if (root == NULL) return;
	f[root->nodeType-FUNC_OFFSET](root);			
	for (p = root->leftChild; p != NULL; p = p->rightSibling)
		tree_traversal(p,f);	
}
