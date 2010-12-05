#include "debugger.h"
#include "string.h"
#include "breakpoint.h"
#include "pipline.h"
#include "process.h"
#include "memory.h"
#include "console.h"

int console_step(CPU_d* cpu);
int console_next(CPU_d* cpu);
int console_run(CPU_d* cpu, char* filename, int mode);
int console_info(CPU_d* cpu, char* cmd);
int console_modify(CPU_d* cpu, char* cmd);
int console_bp(CPU_d* cpu, char* cmd);
int console_help(char* cmd);
int process_restart(CPU_d* cpu, char* filename);

int console_next_cmd(CPU_d* cpu, char* filename){
  char input[200]={0};
  char cmd[20]={0};
  printf(">");
  fflush(stdin);
  fgets(input, 200, stdin);
  sscanf(input, "%s", cmd);
  if(strlen(cmd) == 0)
    return 1;
  if(strcmp(cmd, "q") == 0 || strcmp(cmd, "quit") == 0)
    return 0;
  else if(strcmp(cmd, "h") == 0 || strcmp(cmd, "help") == 0)
    console_help(input);
  else if(strcmp(cmd, "s") == 0 || strcmp(cmd, "step") == 0)
    console_step(cpu);
  else if(strcmp(cmd, "r") == 0 || strcmp(cmd, "run") == 0)
    console_run(cpu, filename, 0);
  else if(strcmp(cmd, "rt") == 0 || strcmp(cmd, "runtrap") == 0)
    console_run(cpu, filename, 1);
  else
    console_help("help");
  return 1;
}

int console_help(char* cmd){
  char argv1[50] = {0};
  sscanf(cmd, "%s %s", argv1, argv1);
  if(strcmp(argv1, "i") == 0 || strcmp(argv1, "info") == 0){
    printf("Usage of command \"info\" :\n");
  }
  else if(strcmp(argv1, "m") == 0 || strcmp(argv1, "modify") == 0){
    printf("Usage of command \"modify\" :\n");
  }
  else if(strcmp(argv1, "b") == 0 || strcmp(argv1, "breakpoint") == 0){
    printf("Usage of command \"breakpoint\" :\n");
  }
  else{
    printf("Simulator Debugger Command List:\n");
    printf("h  :  show help message\n");
    printf("q  :  quit simulator\n");
    printf("r  :  run simulator\n");
    printf("rt :  run in trap mode\n");
    printf("s  :  step next instruction\n");
    printf("n  :  step until next breakpoint or the end of program\n");
    printf("i  :  show informations, see details \"help i\" or \"help info\"\n");
    printf("m  :  modify memory or registers, see details \"help m\" or \"help modify\"\n");
    printf("b  :  breakpoint handling, see details \"help b\" or \"help breakpoint\"\n");
    printf("\n");
  }
  return 1;
}

int console_step(CPU_d* cpu){
  if(cpu->mode != CPU_TRAP && cpu->mode != CPU_NORMAL){
    printf("Program is not run.\n");
    return 0;
  }
  pipline_next_step(cpu->pipline, cpu->cpu_info);
  return 1;
}

// mode - 0 : normal mode
//        1 : trap mode
int console_run(CPU_d* cpu, char* filename, int mode){
  if(cpu->mode == CPU_NORMAL || cpu->mode == CPU_TRAP){
    printf("Progress is running, restart?(Y/N) ");
    char tmp[20]={0};
    scanf("%s", tmp);
    if(strcmp(tmp, "Y") == 0 || strcmp(tmp, "y") == 0 ||
       strcmp(tmp, "yes") == 0 || strcmp(tmp, "YES") == 0){
      process_restart(cpu, filename);
      return 1;
    }
    else
      return 0;
  }else if(cpu->mode == CPU_STOP)
    process_restart(cpu, filename);
  else if(cpu->mode == CPU_INIT){
    cpu->mode = CPU_NORMAL;
    if(mode == 0)
      console_next(cpu);
    else
      console_step(cpu);
  }
  return 1;
}

int process_restart(CPU_d* cpu, char* filename){
  PROCESS* proc;
  proc_destroy(cpu->proc);
  proc = proc_initial(filename);
  reg_destroy(cpu->regs);
  cpu->regs = reg_initial();
  cache_destroy(cpu->i_cache);
  cpu->i_cache = cache_initial(NULL);
  cache_destroy(cpu->d_cache);
  cpu->d_cache = cache_initial(NULL);
  
  CPU_load_process(cpu, proc);
  cpu->mode = CPU_NORMAL;
  memset(cpu->cpu_info, 0, sizeof(CPU_info));
  pipline_destroy(cpu->pipline);
  cpu->pipline = pipline_initial(cpu->regs, cpu->i_cache, cpu->d_cache);

  return 1;
}

CPU_d* process_start(char* filename){
  PROCESS* proc;
  CPU_d* cpu;
  proc = proc_initial(filename);
  cpu = CPU_initial(1, CPU_INIT);
  CPU_load_process(cpu, proc);
  cpu->mode = CPU_INIT;
  return cpu;
}

void process_end(CPU_d* cpu){
  CPU_destroy(cpu);
  proc_destroy(cpu->proc);
}

int console_next(CPU_d* cpu){
  printf("console next\n");
  console_step(cpu);
  return 1;
}
