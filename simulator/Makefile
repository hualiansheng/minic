OBJECT = simulator.o ELF_parser.o register.o memory.o process.o instructions.o cache.o
ARGS = -g -Wall
LIBS = -L/usr/local/lib -lelf
simulator:$(OBJECT)
	gcc $(ARGS) $(LIBS) -o simulator $(OBJECT) 
simulator.o:simulator.c
	gcc $(ARGS) -c simulator.c
ELF_parser.o:ELF_parser.h ELF_parser.c
	gcc $(ARGS) $(LIBS) -c ELF_parser.c
register.o:register.h register.c
	gcc $(ARGS) -c register.c
memory.o:memory.h memory.c
	gcc $(ARGS) -c memory.c
process.o:process.h process.c ELF_parser.h
	gcc $(ARGS) -c process.c
instructions.o:instructions.h instructions.c register.h
	gcc $(ARGS) -c instructions.c
cache.o:cache.h cache.c
	gcc $(ARGS) -c cache.c

clean:
	rm -f *.o simulator