OBJECT = calc.tab.o lex.yy.o symrec.o


calc: $(OBJECT)
	gcc -lm -o calc $(OBJECT)
calc.tab.o: calc.tab.c calc.tab.h
	gcc -c calc.tab.c
lex.yy.o: lex.yy.c calc.tab.h
	gcc -c lex.yy.c
calc.tab.c: calc.y symrec.h
	bison -d calc.y
calc.tab.h: calc.y
	bison -d calc.y
lex.yy.c: calc.l symrec.h
	flex calc.l
symrec.o: symrec.c symrec.h
	gcc -c symrec.c
clean:
	rm *.o; rm calc; rm yy.lex.c; rm calc.tab.*
