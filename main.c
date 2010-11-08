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
	-v Print type verification debug output\n\
	-a All of above = -tdsv\n");
}
int main(int argc, char** argv)
{	
	//CLI arguments
	int oc,
		dbg_print_tree = 0,
		dbg_print_symtbl = 0,
		dbg_type_check = 0,
		dbg_print_tree_dot = 0;
	int gen_symtbl_result ;
	FILE* source_file;
	//error number, handled by bison
	error_number = 0;
	//symtable support: scope number
	scope_number = 0;
	//yydebug = 1;
	while((oc = getopt(argc, argv, "dotsva")) != -1)
	{
		switch(oc)
		{
			case 'd': // debug on
				yydebug = 1;
				break;
			case 'o': //output AST
				dbg_print_tree_dot = 1;
				break;
			case 't':
				dbg_print_tree = 1;
				break;
			case 's':
				dbg_print_symtbl = 1;
				break;
			case 'v':
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
	if (yyparse() || error_number)
	{
		fprintf(stderr, "Parser: terminated, %d error(s).\n",error_number);
		return -1;
	}
	fprintf(stderr,"Generating symbol table...\n");
	//generate symbol table
	gen_symtbl_result = gen_symtbl(tree_root);
	if(gen_symtbl_result)
	{
		fprintf(stderr,"Error generating symbol tables!\n");
		return -1;
	}
	if(!error_number)
	{
		fprintf(stderr,"Error type verification!\n");
		return -1;
	};
	gen_intermediate_code(tree_root);
	/*
	 * TODO: next compilation step
	 */
	if(dbg_print_tree)
		print_AST(tree_root,0);
	if(dbg_print_tree_dot)
		print_AST_dot(tree_root);
	if(dbg_print_symtbl)
		print_symtbl(tree_root->symtbl);
	return 0;
}
