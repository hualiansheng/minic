#include <stdio.h>
#include <err.h>
#include <sysexits.h>

int main(int argc, char **argv){
  if(argc !=2)
    errx(EX_USAGE, "usage: %s file-name", argv[0]);
  Elf_parser(argv[1]);
  return 0;
}
