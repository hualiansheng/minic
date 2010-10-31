simulator:simulator.o ELF_parser.o
	gcc -o simulator simulator.o ELF_parser.o -lelf
simulator.o:simulator.c
	gcc -c simulator.c
ELF_parser.o:ELF_parser.c
	gcc -c ELF_parser.c
clean:
	rm -f *.o