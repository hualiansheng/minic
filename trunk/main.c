#include "validation_utils.h"
#include "main.h"
#include <assert.h>

int main(int argc, char** argv)
{	
	//symtable support: scope number
	scope_number = 0;
	//yydebug = 1;
	FILE* source_file;
	argc--,argv++;
	if(argc)
		source_file = fopen (argv[0], "r" );
	assert( source_file != NULL );
	yyin = source_file;
	yyparse();
	fclose(source_file);
	//print_AST(tree_root, 0);
	gen_symtbl(tree_root);
	print_symtbl(tree_root);
	return 0;
}
