#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <gelf.h>


int fd;
Elf *e;  
GElf_Ehdr ehdr;
int phdr_index;


int Elf_initial(char *input_file){
  if(elf_version(EV_CURRENT) == EV_NONE){
    fprintf(stderr, "ELF library initialization failed: %s.\n",
	    elf_errmsg(-1));
    return 1;
  }
  if((fd = open(input_file, O_RDONLY, 0)) < 0){
    fprintf(stderr, "Open \"%s\" failed.\n", input_file);
    return 2;
  }
  if((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL){
    fprintf(stderr, "elf_begin() failed: %s.\n", elf_errmsg(-1));
    return 3;
  }
  if(elf_kind(e) != ELF_K_ELF){
    fprintf(stderr, "\"%s\" is not an ELF object.\n", input_file);
    return 4;
  }
  if(gelf_getehdr(e, &ehdr) == NULL){
    fprintf(stderr, "getehdr() failed: %s.\n",
	    elf_errmsg(-1));
    return 5;
  }
  if(ehdr.e_flags != 2){
    fprintf(stderr, "\"%s\" is not an execution file.\n", input_file);
    return 6;
  }

  phdr_index = 0;

  // printf("Entry point: 0x%jx\n", ehdr.e_entry);
  // printf("Flags: %d\n", ehdr.e_flags);
  return 0;
}

int Elf_close(){
  (void) elf_end(e);
  (void) close(fd);
  return 1;
}

//get next loadable program header
int Elf_next_loadable_phdr(GElf_Phdr *phdr){
  int phdr_num = ehdr.e_phnum;
  while(phdr_index < phdr_num){
    if(gelf_getphdr(e, phdr_index, phdr) != phdr){
      fprintf(stderr, "getphdr() failed: %s.", elf_errmsg(-1));
      return 0;
    }
    phdr_index ++;
    if(phdr->p_type == PT_LOAD)
      return 1;
  }
  phdr_index = 0;
  return 0;
}

//get elf file entry address
uint32_t Elf_entry_point(){
  return ehdr.e_entry;
}
