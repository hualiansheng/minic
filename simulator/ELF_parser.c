#include <err.h>
#include <fcntl.h>
#include <libelf.h>
#include <gelf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

int i, fd;

Elf *e;
char *id, bytes[5];
size_t n;

GElf_Phdr phdr;

void print_ptype ( size_t pt )
{
  char * s ;
# define C(V) case PT_##V: s = #V; break
    switch ( pt ) {
      C(NULL);
      C(LOAD);
      C(INTERP);
      C(NOTE);
      C(PHDR);
      C(TLS);
      C(SUNWBSS);
      C(SUNWSTACK);
    default:
      s =  "unknown" ;
      break;
    }
    ( void ) printf ( " \"%s\" " , s );
# undef C
}


void Elf_parser(char *input_path){
  if(elf_version(EV_CURRENT) == EV_NONE)
    errx(EX_SOFTWARE, "ELF library initialization failed: %s", 
	 elf_errmsg(-1));

  if((fd = open(input_path, O_RDONLY, 0)) < 0)
    err(EX_NOINPUT, "open \"%s\" failed", input_path);

  if((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
    errx(EX_SOFTWARE, "elf_begin() failed: %s.",
	 elf_errmsg(-1));

  if(elf_kind(e) != ELF_K_ELF)
    errx(EX_DATAERR, "\"%s\" is not an ELF object.", input_path);
  
  if ( elf_getphdrnum (e , &n ) != 0)
    errx ( EX_DATAERR , " elf_getphdrnum () failed : %s . " ,
	   elf_errmsg ( -1));
  
  for(i=0;i<n;i++){
    if(gelf_getphdr(e, i, &phdr)!=&phdr)
      errx(EX_SOFTWARE, "getphdr() failed: %s.",
	   elf_errmsg(-1));
    printf("PHDR %d:\n",i);
    printf("    p_type 0x%jx ", (uintmax_t)phdr.p_type);
    print_ptype(phdr.p_type);
    printf("\n");
    printf("    p_offset 0x%jx \n", (uintmax_t)phdr.p_offset);
    printf("    p_vaddr 0x%jx \n", (uintmax_t)phdr.p_vaddr);
    printf("    p_paddr 0x%jx \n", (uintmax_t)phdr.p_paddr);
  }
  elf_end(e);
  close(fd);
  exit(EX_OK);
}
