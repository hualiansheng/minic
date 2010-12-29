OBJECT = simulator.o ELF_parser.o register.o mem.o process.o decode.o cache.o CPU.o pipline.o debugger.o instEx.o breakpoint.o console.o interpret.o
ARGS = -O2 -Wall 
LIBS = -L/usr/local/lib -lelf
simulator:$(OBJECT)
	gcc $(ARGS) $(LIBS) -o simulator $(OBJECT)
simulator.o:simulator.c config.h console.o
	gcc $(ARGS) -c simulator.c
ELF_parser.o:ELF_parser.h ELF_parser.c
	gcc $(ARGS) $(LIBS)  -c ELF_parser.c
register.o:register.h register.c
	gcc $(ARGS) -c register.c
mem.o:mem.h mem.c
	gcc $(ARGS) -c mem.c
process.o:process.h process.c ELF_parser.h ELF_parser.o breakpoint.h breakpoint.o mem.h mem.o
	gcc $(ARGS) -c process.c
decode.o:decode.h decode.c register.h register.o inst.h pipline.h
	gcc $(ARGS) -c decode.c
cache.o:cache.h cache.c mem.h mem.o
	gcc $(ARGS) -c cache.c
CPU.o:CPU.h CPU.c register.h register.o pipline.h pipline.o debugger.h debugger.o process.h process.o cache.h cache.o
	gcc $(ARGS) -c CPU.c
pipline.o:pipline.h pipline.c interpret.h interpret.o instEx.h instEx.o debugger.h debugger.o decode.h decode.o cache.h cache.o register.h register.o process.h process.o
	gcc $(ARGS) -c pipline.c
debugger.o:debugger.h debugger.c CPU.h
	gcc $(ARGS) -c debugger.c
instEx.o:instEx.h instEx.c pipline.h
	gcc $(ARGS) -c instEx.c
breakpoint.o:breakpoint.h breakpoint.c
	gcc $(ARGS) -c breakpoint.c
console.o:console.h console.c CPU.h CPU.o debugger.h debugger.o breakpoint.h breakpoint.o pipline.h pipline.o process.h process.o interpret.h interpret.o
	gcc $(ARGS) -c console.c
interpret.o:interpret.h interpret.c process.h process.o decode.h decode.o debugger.h debugger.o
	gcc $(ARGS) -c interpret.c

clean:
	rm -f *.o simulator

