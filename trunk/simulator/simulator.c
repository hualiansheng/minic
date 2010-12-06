#include <stdio.h>
#include <stdlib.h>
#include <gelf.h>

#include "console.h"

int main(int argc, char **argv){
  if(argc !=2){
    fprintf(stderr, "usage: %s file-name\n", argv[0]);
    exit(1);
  }
  CPU_d* cpu;
  int sig;
  cpu = process_start(argv[1]);

  printf("Simulator begin.\n");
  printf("File : %s\n", argv[1]);
  printf("CPU mode : normal\n");
  printf("=================================\n");
  while(1){
    sig = console_next_cmd(cpu, argv[1]);
    if(sig == 0)
      break;
  }
  printf("=================================\n");
  printf("Simulator end.\n");

  process_end(cpu);
  
  return 0;
}
