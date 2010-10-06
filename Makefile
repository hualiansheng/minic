OBJECT = minic.tab.o lex.yy.o AST_operation.o main.o validation_utils.o gen_symtbl.o symtbl_operation.o
ARGS = -g -Wall
minic: $(OBJECT)
	gcc -lm $(ARGS) -o minic $(OBJECT)
minic.tab.o: minic.tab.c minic.tab.h AST.h attr.h
	gcc $(ARGS) -c minic.tab.c
lex.yy.o: lex.yy.c minic.tab.h
	gcc $(ARGS) -c lex.yy.c
minic.tab.c: minic.y
	bison -d -t minic.y
minic.tab.h: minic.y
	bison -d -t minic.y
lex.yy.c: minic.l 
	flex minic.l
AST_operation.o: AST.h AST_operation.c
	gcc $(ARGS) -c AST_operation.c
validation_utils.o : validation_utils.c AST.h validation_utils.h
	gcc $(ARGS) -c validation_utils.c
gen_symtbl.o: gen_symtbl.c symtbl.h
	gcc $(ARGS) -c gen_symtbl.c
symtbl_operation.o: symtbl_operation.c
	gcc $(ARGS) -c symtbl_operation.c
main.o : main.c validation_utils.h main.h
	gcc $(ARGS) -c main.c


clean:
	rm *.o; rm minic; rm lex.yy.c; rm minic.tab.*
cleanw:
	del *.o; rm minic; rm lex.yy.c; rm minic.tab.*
