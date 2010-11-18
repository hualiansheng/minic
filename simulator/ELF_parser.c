#include "ELF_parser.h"
#include <malloc.h>
#include <string.h>

int fd;
Elf *e;  
GElf_Ehdr ehdr;
int phdr_index;
GElf_Shdr shdr;
Elf_Scn *scn;
Elf_Data* data;


int ELF_initial(char *input_file){
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

int ELF_close(){
  (void) elf_end(e);
  (void) close(fd);
  return 1;
}

//get loadable segment num
int ELF_loadable_seg_num(){
  GElf_Phdr phdr;
  int result = 0;
  int phdr_num = ehdr.e_phnum;
  while(phdr_index < phdr_num){
    if(gelf_getphdr(e, phdr_index, &phdr) != &phdr){
      fprintf(stderr, "getphdr() failed: %s.", elf_errmsg(-1));
      phdr_index = 0;
      return -1;
    }
    phdr_index ++;
    if(phdr.p_type == PT_LOAD)
      result++;
  }
  phdr_index = 0;
  return result;
}

//get next loadable program header
GElf_Phdr* ELF_next_loadable_phdr(){
  GElf_Phdr *phdr = malloc(sizeof(GElf_Phdr));
  int phdr_num = ehdr.e_phnum;
  while(phdr_index < phdr_num){
    if(gelf_getphdr(e, phdr_index, phdr) != phdr){
      phdr_index = 0;
      fprintf(stderr, "getphdr() failed: %s.", elf_errmsg(-1));
      return NULL;
    }
    phdr_index ++;
    if(phdr->p_type == PT_LOAD)
      return phdr;
  }
  phdr_index = 0;
  return NULL;
}

//get elf file entry address
uint32_t ELF_entry_point(){
  return ehdr.e_entry;
}

uint32_t ELF_main_entry(){
  uint32_t result = 0;
  uint32_t sym_num;
  Elf32_Sym * sym;
  int strtab_index = 0;
  scn = NULL;
  while((scn = elf_nextscn(e, scn)) != NULL){
    if(gelf_getshdr(scn, &shdr) != &shdr){
      fprintf(stderr, "getshdr() failed.\n");
      return 0;
    }
    strtab_index ++;
    char* name = elf_strptr(e, ehdr.e_shstrndx, shdr.sh_name);
    if(strcmp(name, ".strtab")==0)
      break;
  }

  scn = NULL;
  while((scn = elf_nextscn(e, scn)) != NULL){
    if(gelf_getshdr(scn, &shdr) != &shdr){
      fprintf(stderr, "getshdr() failed.\n");
      return 0;
    }
    char* name = elf_strptr(e, ehdr.e_shstrndx, shdr.sh_name);
    if(strcmp(name, ".symtab")==0){
      data = elf_getdata(scn, data);
      sym_num = data->d_size/sizeof(Elf32_Sym);
      sym = (Elf32_Sym*)data->d_buf;
      int i;
      for(i=0; i<sym_num; i++){
	name = elf_strptr(e, strtab_index, sym[i].st_name);
	if(strcmp(name, "main") == 0){
	  result = sym[i].st_value;
	  return result;
	}
      }
    }
  }
  return result;
}

