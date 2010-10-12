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
void usage()
{
	printf("MiniC compiler:\n\
Usage: minic [option] source_file\n\
Options:\n\
	-d Bison parser debug output\n\
	-t Print AST\n\
	-s Print symbol tables\n\
	-a All of above = -tds\n");
}
int main(int argc, char** argv)
{	
	//CLI arguments
	int oc,
		dbg_print_tree = 0,
		dbg_print_symtbl = 0,
		dbg_type_check = 0;
	FILE* source_file;
	//symtable support: scope number
	scope_number = 0;
	//yydebug = 1;
	while((oc = getopt(argc, argv, "dtsca")) != -1)
	{
		switch(oc)
		{
			case 'd': // debug on
				yydebug = 1;
				break;
			case 't': //output AST
				dbg_print_tree = 1;
				break;
			case 's':
				dbg_print_symtbl = 1;
				break;
			case 'c':
				dbg_type_check = 1;
				break;
			case 'a':
				yydebug = dbg_print_tree
					= dbg_print_symtbl 
					= dbg_type_check 
					= 1;
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
	yyparse();
	//generate symbol table
	gen_symtbl(tree_root);
	/*
	 * TODO: next compilation step
	 */
	if(dbg_print_tree)
		print_AST(tree_root,0);
	if(dbg_print_symtbl)
		print_symtbl(tree_root);
	if(dbg_type_check)
		/*
		 * TODO: type checking debug output.
		 */
		;
	return 0;
}
