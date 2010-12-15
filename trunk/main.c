/**
 * Caution:
 * Due to the dependency on unistd.h, 
 * only under POSIX OS can this file be compiled.
 * To use minic under Windows, compile main.c.old
 */
#include "validation_utils.h"
#include "main.h"
#include "register_stats.h"
#include <unistd.h> //getopt() support
#include <assert.h>
#include <string.h>
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
	-a All of above = -tds\n\
	----------------------------------\n\
	Target machine register system info (corresponding with ABI):\n\
		Caller save: r%d - r%d\n\
		Callee save: r%d - r%d\n", CALLER_REG_START,
	CALLER_REG_END,
	CALLEE_REG_START,
	CALLEE_REG_END);
}
/**
 * parse the input file name, cut out the extend name.
 */
void parse_file_name(char* dst, char* src)
{
	int i, len = strlen(src);
	memset(dst,0,sizeof(char) * (len+1));
	for (i = len - 1; i >= 0 && src[i] != '.'; i--)
		;
	strncpy(dst,src,i<0?len:i);
}
int main(int argc, char** argv)
{
	//CLI arguments
	char srcfile_name[255];
	int oc,
		dbg_print_tree = 0,
		dbg_print_symtbl = 0,
		dbg_print_tree_dot = 0,
		dbg_no_intermediate = 0,
		dbg_print_basic_block = 0,
		dbg_print_interference_graph=0,
		dbg_print_intermediate_code = 0,
		dbg_print_live_var = 0,
		dbg_print_register_allocation = 0;
#ifdef DEBUG
	dbg_print_live_var = 1;
	dbg_print_register_allocation = 1;
#endif
	int gen_symtbl_result ;
	FILE *source_file, *target_file;
	//error number, handled by bison
	error_number = 0;
	//symtable support: scope number
	scope_number = 0;
	//yydebug = 1;
	while((oc = getopt(argc, argv, "dotsgaxiblr")) != -1)
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
			case 'l':
				dbg_print_live_var = 1;
				break;
			case 'r':
				dbg_print_register_allocation = 1;
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
	parse_file_name(srcfile_name, argv[optind]);
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
	
	//generate symbol table
	fprintf(stderr,"Generating symbol table...");
	gen_symtbl_result = gen_symtbl(tree_root);
	if(gen_symtbl_result)
	{
		fprintf(stderr,"Error generating symbol tables!\n");
		return -1;
	}
	fprintf(stderr,"done.\n");

	if(dbg_print_symtbl)
		print_symtbl(tree_root->symtbl);

	//type verification
	fprintf(stderr,"Doing type verification...");
	dfs_type_verification(tree_root);
	fprintf(stderr,"done.\n");
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

	//generate intermediate code
	fprintf(stderr,"Generating intermediate code...");
	intermediate_code(tree_root);
	fprintf(stderr,"done.\n");
	if(dbg_print_intermediate_code)
	{
		print_intermediate_code();
		return 0;
	}

	//generate basic block
	fprintf(stderr,"Generating basic block...");
	gen_basic_block();
	fprintf(stderr,"done.\n");
	if(dbg_print_basic_block)
	{	
		print_basic_block();
		return 0;
	}	
	//live variable analyzing
	fprintf(stderr,"Analyzing lively variables...");
	live_var_anal();
	fprintf(stderr,"done.\n");
	/**
	 *DEBUG FILE:
	 *result file of live variable analizing is live_var_anal.debug
	 */
	if(dbg_print_live_var)
		print_live_var();
	if(dbg_print_interference_graph)
	{	
		print_interference_graph();
		return 0;
	}
	//register allocation
	fprintf(stderr,"Allocating registers for variables...");
	register_allocation(AVALIABLE_REG_NUM);
	fprintf(stderr,"done.\n");
	/**
	 *DEBUG FILE:
	 *register allocation debug file is register_allocation.debug
	 */
	if(dbg_print_register_allocation)
		print_register_allocation();
		
	/*
	 * TODO: Optimizing
	 */
	fprintf(stderr,"Generating target code...");
	gen_target_code();
	fprintf(stderr,"done.\n");
	target_file = stdout;
#ifndef DEBUG
	target_file = fopen(strcat(srcfile_name,".s"),"w+");
#endif
	print_target_code(target_file);
	fclose(target_file);
	return 0;
}
