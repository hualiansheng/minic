#ifndef _ELF_PARSER_H
#define _ELF_PARSER_H_

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <gelf.h>

extern int ELF_initial(char *input_file);
extern int ELF_close();
extern int ELF_loadable_seg_num();
extern GElf_Phdr* ELF_next_loadable_phdr();
extern uint32_t ELF_entry_point();

#endif
