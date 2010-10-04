#include "AST.h"
#include <stdio.h>

struct AST_NODE* AST_new_node(){
  struct AST_NODE* result = (struct AST_NODE *)malloc(sizeof(struct AST_NODE));
  result->scopeIndex = -1;
  result->father = NULL;
  result->leftChild = NULL;
  result->rightSibling = NULL;
  return result;
}

struct AST_NODE* AST_initial(){
  struct AST_NODE* result = AST_new_node();
  result->nodeType = PROGRAM;
  result->nodeLevel = 0;
  return result;
}

void AST_addLeftChild(struct AST_NODE* root, 
		      int _nodeType, char* _nodeContent){
  root->leftChild = AST_new_node();
  root->leftChild->nodeType = _nodeType;
  root->leftChild->nodeContent = _nodeContent;
  root->leftChild->nodeLevel = root->nodeLevel + 1;
}


//test main
int main(){
  struct AST_NODE* AST_Root=AST_initial();
  printf("%d", AST_Root->nodeType);
  free(AST_Root);
}
