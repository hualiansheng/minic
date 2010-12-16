#ifndef _ELF_PARSER_H
#define _ELF_PARSER_H_

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <gelf.h>

/* Legal values for ST_TYPE subfield of st_info (symbol type).  */
/* From elf.h*/
// get symbal table ST_TYPE from st_info
#define ELF32_ST_TYPE(val)		((val) & 0xf)

#define STT_NOTYPE	0		/* Symbol type is unspecified */
#define STT_OBJECT	1		/* Symbol is a data object */
#define STT_FUNC	2		/* Symbol is a code object */
#define STT_SECTION	3		/* Symbol associated with a section */
#define STT_FILE	4		/* Symbol's name is file name */
#define STT_COMMON	5		/* Symbol is a common data object */
#define STT_TLS		6		/* Symbol is thread-local data object*/
#define	STT_NUM		7		/* Number of defined types.  */
#define STT_LOOS	10		/* Start of OS-specific */
#define STT_GNU_IFUNC	10		/* Symbol is indirect code object */
#define STT_HIOS	12		/* End of OS-specific */
#define STT_LOPROC	13		/* Start of processor-specific */
#define STT_HIPROC	15		/* End of processor-specific */


typedef struct{
    int sym_num;
    unsigned char* st_info;
    char** name;
    uint32_t* addr;
}PROC_SYMTBL;

int ELF_initial(char *input_file);
int ELF_close();
int ELF_loadable_seg_num();
GElf_Phdr* ELF_next_loadable_phdr();
uint32_t ELF_entry_point();
uint32_t ELF_main_entry();
int ELF_build_symtbl(PROC_SYMTBL* symtbl);

#endif
