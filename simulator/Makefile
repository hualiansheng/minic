OBJECT = simulator.o ELF_parser.o register.o mem.o process.o decode.o cache.o CPU.o pipline.o debugger.o instEx.o
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
mem.o:mem.h mem.c
	gcc $(ARGS) -c mem.c
process.o:process.h process.c ELF_parser.h
	gcc $(ARGS) -c process.c
decode.o:decode.h decode.c register.h inst.h
	gcc $(ARGS) -c decode.c
cache.o:cache.h cache.c
	gcc $(ARGS) -c cache.c
CPU.o:CPU.h CPU.c
	gcc $(ARGS) -c CPU.c
pipline.o:pipline.h pipline.c
	gcc $(ARGS) -c pipline.c
debugger.o:debugger.h debugger.c
	gcc $(ARGS) -c debugger.c
instEx.o:instEx.h instEx.c pipline.h
	gcc $(ARGS) -c instEx.c

clean:
	rm -f *.o simulator
