/**
 * Caution:
 * Due to the dependency on unistd.h, 
 * only under POSIX OS can this file be compiled.
 * To use minic under Windows, compile main.c.old
 */
#include "validation_utils.h"
#include "main.h"
#include <assert.h>

#include <unistd.h> //getopt() support
int error_number;
void usage()
{
	printf("MiniC compiler:\n\
Usage: minic [options] source_file\n\
Options:\n\
	-d Bison parser debug output\n\
	-o Print AST in a DOT file\n\
	-t Print ASCII AST tree\n\
	-s Print symbol tables\n\
	-i Print intermediate code\n\
	-b Print basic block in a DOT file\n\
	-g Print interference graph\n\
	-a All of above = -tds\n");
}
int main(int argc, char** argv)
{	
	//CLI arguments
	int oc,
		dbg_print_tree = 0,
		dbg_print_symtbl = 0,
		dbg_print_tree_dot = 0,
		dbg_no_intermediate = 0,
		dbg_print_basic_block = 0,
		dbg_print_interference_graph=0,
		dbg_print_intermediate_code = 0;
	int gen_symtbl_result ;
	FILE* source_file;
	//error number, handled by bison
	error_number = 0;
	//symtable support: scope number
	scope_number = 0;
	//yydebug = 1;
	while((oc = getopt(argc, argv, "dotsgaxib")) != -1)
	{
		switch(oc)
		{
			case 'd': // debug on
				yydebug = 1;
				break;
			case 'o': //output AST
				dbg_print_tree_dot = 1;
				break;
			case 's':
				dbg_print_symtbl = 1;
				break;
			case 'a':
				yydebug = dbg_print_tree
					= dbg_print_symtbl 
					= 1;
				break;
			case 'x':
				dbg_no_intermediate = 1;
				break;
			case 't':
				dbg_print_tree = 1;
				dbg_print_intermediate_code = 0;
				dbg_print_basic_block = 0;
				dbg_print_interference_graph = 0;
				break;
			case 'i':
				dbg_print_tree = 0;
				dbg_print_basic_block = 0;
				dbg_print_interference_graph = 0;
				dbg_print_intermediate_code = 1;
				break;
			case 'b':
				dbg_print_intermediate_code = 0;
				dbg_print_tree = 0;
				dbg_print_interference_graph = 0;
				dbg_print_basic_block = 1;
				break;
			case 'g':
				dbg_print_intermediate_code = 0;
				dbg_print_tree = 0;
				dbg_print_interference_graph = 1;
				dbg_print_basic_block = 0;
				break;
			default:
				usage();
				return -1;
		}
	}
	if(optind >= argc) // no file name given
	{
		usage();
		return -1;
	}
	
	source_file = fopen (argv[optind], "r");
	if (!source_file) //file open error
	{
		fprintf(stderr,"%s: open error!\n",argv[optind]);
		return -1;
	}
	yyin = source_file;
	//call bison parser
	if (yyparse() || error_number)
	{
		fprintf(stderr, "Parser: terminated, %d error(s).\n",error_number);
		return -1;
	}
	
	if(dbg_print_tree)
		print_AST(tree_root,0);
	if(dbg_print_tree_dot)
		print_AST_dot(tree_root);
	
	fprintf(stderr,"Generating symbol table...");
	//generate symbol table
	gen_symtbl_result = gen_symtbl(tree_root);
	if(gen_symtbl_result)
	{
		fprintf(stderr,"Error generating symbol tables!\n");
		return -1;
	}
	
	fprintf(stderr,"Done.\n");
	if(dbg_print_symtbl)
		print_symtbl(tree_root->symtbl);
	fprintf(stderr,"Doing type verification...");
	dfs_type_verification(tree_root);
	fprintf(stderr,"Done.\n");
	if(error_number)
	{
		fprintf(stderr,"Error type verification!\n");
		return -1;
	};
	if(dbg_no_intermediate)
	{
		fprintf(stderr,"No intermediate mode: i am quiting...\n");
		return 0;
	}
	intermediate_code(tree_root);
	if(dbg_print_intermediate_code)
	{
		print_intermediate_code();
		return 0;
	}	
	gen_basic_block();
	if(dbg_print_basic_block)
	{	
		print_basic_block();
		return 0;
	}	
	live_var_anal();
	print_live_var();
	if(dbg_print_interference_graph)
	{	
		print_interference_graph();
		return 0;
	}
	printf("allocating...\n");
	register_allocation(18);
	print_register_allocation();
	gen_target_code();
	print_target_code();
	/*
	 * TODO: next compilation step
	 */
	return 0;
}
