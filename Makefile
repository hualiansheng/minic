OBJECT = minic.tab.o lex.yy.o AST_operation.o
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


clean:
	rm *.o; rm minic; rm yy.lex.c; rm minic.tab.*
