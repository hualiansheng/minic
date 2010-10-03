OBJECT = minic.tab.o lex.yy.o
ARGS = -g
minic: $(OBJECT)
	gcc -lm $(ARGS) -o minic $(OBJECT)
minic.tab.o: minic.tab.c minic.tab.h
	gcc $(ARGS) -c minic.tab.c
lex.yy.o: lex.yy.c minic.tab.h
	gcc $(ARGS) -c lex.yy.c
minic.tab.c: minic.y
	bison -d -t minic.y
minic.tab.h: minic.y
	bison -d -t minic.y
lex.yy.c: minic.l 
	flex minic.l

clean:
	rm *.o; rm minic; rm yy.lex.c; rm minic.tab.*
