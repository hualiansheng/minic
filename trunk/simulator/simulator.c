#include <stdio.h>
#include <gelf.h>

#include "register.h"
#include "memory.h"

int main(int argc, char **argv){
  /* ELF parser module
  if(argc !=2)
    fprintf(stderr, "usage: %s file-name", argv[0]);
  Elf_initial(argv[1]);
  Elf_close();
  GElf_Phdr phdr;
  if(Elf_next_loadable_phdr(&phdr) != 0)
    printf("Phdr type : %d\n", phdr.p_type);
  */
  REGISTERS regs;
  reg_initial(&regs);

  return 0;
}
