#include "debugger.h"
#include "string.h"
#include "breakpoint.h"
#include "pipline.h"
#include "process.h"
#include "memory.h"
#include "console.h"
#include "interpret.h"

typedef struct{
  char command[20];
  int arg_num;
  char args[30][30];
}CMD;

int console_step(CPU_d* cpu, CMD cmd);// command step/s
int console_next(CPU_d* cpu);// command next/s
int console_run(CPU_d* cpu, char* filename, int mode);// command r/rt
int console_x(CPU_d* cpu, CMD cmd);// command x
int console_print(CPU_d* cpu, CMD cmd);// command print/p
int console_info(CPU_d* cpu, CMD cmd);// command info/i
int console_modify(CPU_d* cpu, CMD cmd);// command modify/m
int console_list(CPU_d* cpu, CMD cmd);// command list/l
int console_bp(CPU_d* cpu, CMD cmd);// command breakpoint/b
int console_help(CMD cmd);// command help/h
int process_restart(CPU_d* cpu, char* filename);// restart process
CMD console_parse_cmd(char* input);// parse command
void console_help_info();// help doc of command info/i
void console_help_modify();// help doc of command modify/m
void console_help_breakpoint();// help doc of command breakpoint/b
void console_help_x();// help doc of command x
void console_help_print();// help doc of command print/p
void console_help_list();// help doc of list/l


int console_next_cmd(CPU_d* cpu, char* filename){
  char input[200]={0};
  CMD cmd;
  printf(">");
  fflush(stdin);
  fgets(input, 200, stdin);
  cmd = console_parse_cmd(input);
  if(strlen(cmd.command) == 0)
    return 1;
  if(strcmp(cmd.command, "q") == 0 || strcmp(cmd.command, "quit") == 0)
    return 0;
  else if(strcmp(cmd.command, "h") == 0 || strcmp(cmd.command, "help") == 0)
    console_help(cmd);
  else if(strcmp(cmd.command, "s") == 0 || strcmp(cmd.command, "step") == 0)
    console_step(cpu, cmd);
  else if(strcmp(cmd.command, "r") == 0 || strcmp(cmd.command, "run") == 0)
    console_run(cpu, filename, 0);
  else if(strcmp(cmd.command, "rt") == 0 ||
	  strcmp(cmd.command, "runtrap") == 0)
    console_run(cpu, filename, 1);
  else if(strcmp(cmd.command, "x") == 0)
    console_x(cpu, cmd);
  else if(strcmp(cmd.command, "p") == 0 ||
	  strcmp(cmd.command, "print") == 0)
    console_print(cpu, cmd);
  else if(strcmp(cmd.command, "i") == 0 ||
	  strcmp(cmd.command, "info") == 0)
    console_info(cpu, cmd);
  else if(strcmp(cmd.command, "n") == 0 ||
	  strcmp(cmd.command, "next") == 0)
    console_next(cpu);
  else if(strcmp(cmd.command, "m") == 0 ||
	  strcmp(cmd.command, "modify") == 0)
    console_modify(cpu, cmd);
  else if(strcmp(cmd.command, "l") == 0 ||
	  strcmp(cmd.command, "list") == 0)
    console_list(cpu, cmd);
  else
    console_help(cmd);
  return 1;
}

int console_help(CMD cmd){
  if(strcmp(cmd.args[0], "i") == 0 || strcmp(cmd.args[0], "info") == 0)
    console_help_info();
  else if(strcmp(cmd.args[0], "m") == 0 ||
	  strcmp(cmd.args[0], "modify") == 0)
    console_help_modify();
  else if(strcmp(cmd.args[0], "b") == 0 ||
	  strcmp(cmd.args[0], "breakpoint") == 0)
    console_help_breakpoint();
  else if(strcmp(cmd.args[0], "i") == 0 ||
	  strcmp(cmd.args[0], "info") == 0)
    console_help_info();
  else if(strcmp(cmd.args[0], "x") == 0)
    console_help_x();
  else if(strcmp(cmd.args[0], "p") == 0 ||
	  strcmp(cmd.args[0], "print") == 0)
    console_help_print();
  else if(strcmp(cmd.args[0], "l") == 0 ||
	  strcmp(cmd.args[0], "list") == 0)
    console_help_list();
  else{
    printf("Simulator Debugger Command List:\n");
    printf("h  :  show help message\n");
    printf("q  :  quit simulator\n");
    printf("r  :  run simulator\n");
    printf("rt :  run in trap mode\n");
    printf("s  :  step next instruction\n");
    printf("n  :  step until next breakpoint or the end of program\n");
    printf("x  :  show the value of the given address, see details \"help x\"\n");
    printf("p  :  print the value, see details \"help p\" or \"help print\"\n");
    printf("l  :  list codes, see details \"help l\" or \"help list\"\n");
    printf("i  :  show informations, see details \"help i\" or \"help info\"\n");
    printf("m  :  modify memory or registers, see details \"help m\" or \"help modify\"\n");
    printf("b  :  breakpoint handling, see details \"help b\" or \"help breakpoint\"\n");
    printf("\n");
  }
  return 1;
}

// return val :  0 - wrong
//               1 - normal
//              -1 - CPU STOP
int console_step(CPU_d* cpu, CMD cmd){
  if(cpu->mode != CPU_TRAP && cpu->mode != CPU_NORMAL){
    printf("Program is not run.\n");
    return 0;
  }
  int num, i;
  if(cmd.arg_num != 0)
    sscanf(cmd.args[0], "%d", &num);
  else
    num = 1;
  if(num <= 0){
    printf("Invalid step argument.\n");
    return 0;
  }
  for(i=0; i<num; i++)
    if(pipline_next_step(cpu->pipline, cpu->cpu_info) == 0){
      cpu->mode = CPU_STOP;
      //printf("CPU STOP\n");
      return -1;
    }
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
      console_run(cpu, filename, 0);
      return 1;
    }
    else
      return 0;
  }else if(cpu->mode == CPU_STOP){
    process_restart(cpu, filename);
    console_run(cpu, filename, mode);
  }
  else if(cpu->mode == CPU_INIT){
    if(mode == 0){
      cpu->mode = CPU_NORMAL;
      console_next(cpu);
    }
    else{
      CMD cmd;
      cmd.arg_num = 0;
      cpu->mode = CPU_TRAP;
      console_step(cpu, cmd);
    }
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
  //printf("0x%.8x\n", (uint32_t)proc);
  cpu->mode = CPU_INIT;
  memset(cpu->cpu_info, 0, sizeof(CPU_info));
  pipline_destroy(cpu->pipline);
  cpu->pipline = pipline_initial(cpu->regs, cpu->i_cache, cpu->d_cache);
  CPU_load_process(cpu, proc);

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

CMD console_parse_cmd(char* input){
  CMD cmd;
  int i, j;
  memset(&cmd, 0, sizeof(cmd));
  i = j = 0;
  cmd.arg_num = 0;
  // parse command
  j = 0;
  while(input[i] == ' ')
    i++;
  while(input[i] != ' ' && input[i] != '\n'){
    cmd.command[j] = input[i];
    i ++;
    j ++;
  }
  while(input[i] == ' ')
    i++;
  cmd.command[j] = '\0';
  // parse args
  while(input[i] != '\n'){
    j = 0;
    while(input[i] != ' ' && input[i] != '\n'){
      cmd.args[cmd.arg_num][j] = input[i];
      i ++;
      j ++;
    }
    cmd.arg_num ++;
    while(input[i] == ' ')
      i++;
  }
  return cmd;
}

int console_print(CPU_d* cpu, CMD cmd){
  int num;
  char tmp[10];
  if(cmd.arg_num == 0){
    console_help_print();
    return 0;
  }
  if(cmd.args[0][0] != '$'){
    printf("%s\n", cmd.args[0]);
    return 1;
  }
  if(cmd.args[0][1] != 'r'){
    printf("Invalid print format. See \"help p\" for more details.\n");
    return 0;
  }
  tmp[0] = cmd.args[0][2];
  tmp[1] = cmd.args[0][3];
  tmp[2] = cmd.args[0][4];
  if(tmp[1] != '\0' && tmp[2] != '\0'){
    printf("Invalid print format. See \"help p\" for more details.\n");
    return 0;
  }
  sscanf(tmp, "%d", &num);
  if(num < 0 || num > 31){
    printf("Invalid print format. See \"help p\" for more details.\n");
    return 0;
  }
  printf("r[%d]  :  0x%.8x\n", num, cpu->regs->r[num]);
  return 1;
}

int console_x(CPU_d* cpu, CMD cmd){
  uint32_t addr;
  if(cmd.arg_num == 0){
    console_help_x();
    return 0;
  }
  if(cmd.args[0][0] == '0' && cmd.args[0][1] == 'x'){
    cmd.args[0][0] = ' ';
    cmd.args[0][1] = ' ';
    sscanf(cmd.args[0], "  %x", &addr);
  }else
    sscanf(cmd.args[0], "%d", &addr);
  debugger_print_mem(cpu->proc->mem, addr);
  return 1;
}

int console_info(CPU_d* cpu, CMD cmd){
  if(cmd.arg_num == 0){
    console_help_info();
    return 0;
  }
  if(strcmp(cmd.args[0], "registers") == 0 ||
     strcmp(cmd.args[0], "register") == 0 ||
     strcmp(cmd.args[0], "r") == 0)
    debugger_print_register(cpu->regs, -1);
  else if(strcmp(cmd.args[0], "stack") == 0 ||
	  strcmp(cmd.args[0], "s") == 0)
    debugger_print_stack(cpu->proc->mem, cpu->regs->REG_SP);
  else
    console_help_info();
  return 1;
}

int console_modify(CPU_d* cpu, CMD cmd){
  if(cmd.arg_num < 3){
    console_help_modify();
    return 0;
  }
  uint32_t addr;
  int32_t content;
  // parse second param, addr or the number of register
  if(cmd.args[1][0] == '0' && cmd.args[1][1] == 'x'){
    cmd.args[1][0] = ' ';
    cmd.args[1][1] = ' ';
    sscanf(cmd.args[1], "  %x", &addr);
  }else
    sscanf(cmd.args[1], "%d", &addr);
  // parse third param, content
  if(cmd.args[2][0] == '0' && cmd.args[2][1] == 'x'){
    cmd.args[2][0] = ' ';
    cmd.args[2][1] = ' ';
    sscanf(cmd.args[2], "  %x", &content);
  }else
    sscanf(cmd.args[2], "%d", &content);
  printf("0x%.8x  0x%.8x\n", addr, content);
  // modify register
  if(strcmp(cmd.args[0], "registers") == 0 ||
     strcmp(cmd.args[0], "register") == 0 ||
     strcmp(cmd.args[0], "r") == 0)
    debugger_modify_register(cpu->regs, addr, content);
  else if(strcmp(cmd.args[0], "memory") == 0 ||
	  strcmp(cmd.args[0], "mem") == 0 ||
	  strcmp(cmd.args[0], "m") == 0){
    debugger_modify_mem(cpu->proc->mem, addr, content);
    cache_update(cpu->i_cache, addr);
    cache_update(cpu->d_cache, addr);
  }
  else
    console_help_modify();
  return 1;
}

int console_list(CPU_d* cpu, CMD cmd){
  uint32_t addr;
  int line_num;
  if(cmd.arg_num == 0){
    addr = cpu->proc->list_cur_addr;
    line_num = 10;
  }
  else if(cmd.arg_num == 1){
    // parse addr
    if(cmd.args[0][0] == 'c')
      addr = cpu->proc->list_cur_addr;
    else if(cmd.args[0][0] == 'i')
      addr = cpu->proc->entry;
    else if(cmd.args[0][0] == '0' && cmd.args[0][1] == 'x'){
      cmd.args[0][0] = ' ';
      cmd.args[0][1] = ' ';
      sscanf(cmd.args[0], "  %x", &addr);
    }else
      sscanf(cmd.args[0], "%d", &addr);
    line_num = 10;
  }
  else{
    // parse addr
    if(cmd.args[0][0] == 'c')
      addr = cpu->proc->list_cur_addr;
    else if(cmd.args[0][0] == 'i')
      addr = cpu->proc->entry;
    else if(cmd.args[0][0] == '0' && cmd.args[0][1] == 'x'){
      cmd.args[0][0] = ' ';
      cmd.args[0][1] = ' ';
      sscanf(cmd.args[0], "  %x", &addr);
    }else
      sscanf(cmd.args[0], "%d", &addr);
    // parse line number
    if(cmd.args[1][0] == '0' && cmd.args[1][1] == 'x'){
      cmd.args[1][0] = ' ';
      cmd.args[1][1] = ' ';
      sscanf(cmd.args[1], "  %x", &line_num);
    }else
      sscanf(cmd.args[1], "%d", &line_num);
  }
  if(line_num < 0){
    printf("Invalid arguent [lines] : %d\n", line_num);
    return 0;
  }
  if(mem_invalid(cpu->proc->mem, addr) != 0){
    printf("Invalid address : 0x%.8x\n", addr);
    return 0;
  }
  if(mem_invalid(cpu->proc->mem, cpu->proc->list_cur_addr) != 0){
    printf("List curse is to the end of segment.\n");
    return 0;
  }
  int i;
  int32_t data;
  char name[100];
  char ass_code[100];
  for(i=0; i<line_num; i++){
    if(mem_invalid(cpu->proc->mem, addr) != 0)
      break;
    mem_fetch(cpu->proc->mem, addr + 4*i, &data, sizeof(data), DATA_RD);
    if(debugger_search_symtbl(cpu->proc, addr + 4*i, name, STT_FUNC) == 1)
      printf("<%s>\n", name);
    interpret_inst(data, addr+4*i, ass_code, cpu->proc);
    printf("0x%.8x  :  0x%.8x\t%s\n", addr+4*i, data, ass_code);
  }
  cpu->proc->list_cur_addr = addr + i*4;
  return 1;
}

void console_help_info(){
  printf("Usage command info:\n");
  printf("info registers : print register heap infomation\n");
  printf("info stack     : printstack infomatio\n");
}

void console_help_x(){
  printf("Usage command x:\n");
  printf("Format   : x arg|0xarg");
  printf("Function : show content of memory\n");
  printf("\tx arg    : arg is the address of memory based on ten.\n");
  printf("\tx 0xarg  : arg is the address of memory based on eight.\n");
}

void console_help_print(){
  printf("Usage command print(p):\n");
  printf("Function : show content of register or other content.\n");
  printf("Format   : print arg|$rx\n");
  printf("\tprint arg : print arg\n");
  printf("\tprint $rx : print content of register, x is the id of register.\n");
}

void console_help_list(){
  printf("Usage command list(l):\n");
  printf("Function : list codes from beginning.\n");
  printf("Format   : list/l [addr] [lines]\n");
  printf("\t[addr]  : the address of codes to display, \"i\" is the begining of program, \"c\" is the current position, \"c\" is default.\n");
  printf("\t[lines] : number of lines to list, 10 is default.\n");
}

void console_help_modify(){
  printf("Usage command modify:\n");
}

void console_help_breakpoint(){
  printf("Usage command breakpoint:\n");
}

//-----------------------------------------------------
// Functions below is to be finished.
int console_next(CPU_d* cpu){
  if(cpu->mode != CPU_TRAP && cpu->mode != CPU_NORMAL){
    printf("Program is not run.\n");
    return 0;
  }
  printf("console next, to be finished.\n");
  cpu->mode = CPU_NORMAL;
  CMD cmd;
  cmd.arg_num = 0;
  while(console_step(cpu, cmd) != -1);
  return 1;
}

int console_bp(CPU_d* cpu, CMD cmd){

  return 1;
}
