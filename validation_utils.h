/**
 * MiniC compiler
 * tool functions for validation for all pheses
 *	
 */
#ifndef VALIDATION_UTILS_H_
#define VALIDATION_UTILS_H_
#include "AST.h"
void print_AST(AST_NODE* ptr, int level);
void print_symtbl(symtbl_hdr* p);
void print_AST_dot(AST_NODE*ptr);
void print_intermediate_code();
void print_register_allocation();
void print_interference_graph();
void print_available_expr();
#endif
