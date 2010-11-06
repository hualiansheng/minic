#include <stdio.h>
#include <gelf.h>

int main(int argc, char **argv){
  if(argc !=2)
    fprintf(stderr, "usage: %s file-name", argv[0]);
  Elf_initial(argv[1]);
  Elf_close();
  GElf_Phdr phdr;
  if(Elf_next_loadable_phdr(&phdr) != 0)
    printf("Phdr type : %d\n", phdr.p_type);
  return 0;
}
