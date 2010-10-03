OBJECT = minic.tab.o lex.yy.o

minic: $(OBJECT)
	gcc -lm -o minic $(OBJECT)
minic.tab.o: minic.tab.c minic.tab.h
	gcc -c minic.tab.c
lex.yy.o: lex.yy.c minic.tab.h
	gcc -c lex.yy.c
minic.tab.c: minic.y
	bison -d minic.y
minic.tab.h: minic.y
	bison -d minic.y
lex.yy.c: minic.l 
	flex minic.l

clean:
	rm *.o; rm minic; rm yy.lex.c; rm minic.tab.*
