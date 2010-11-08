OBJECT = simulator.o ELF_parser.o register.o memory.o
ARGS = -g -Wall
simulator:$(OBJECT)
	gcc $(ARGS) -o simulator -lelf $(OBJECT)
simulator.o:simulator.c
	gcc $(ARGS) -c simulator.c
ELF_parser.o:ELF_parser.c
	gcc $(ARGS) -c ELF_parser.c
register.o:register.h register.c
	gcc $(ARGS) -c register.c
memory.o:memory.h memory.c
	gcc $(ARGS) -c memory.c


clean:
	rm -f *.o