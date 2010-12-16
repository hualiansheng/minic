#include <stdio.h>
#include <stdlib.h>
#include <gelf.h>
#include <memory.h>
//#include <curses.h>
#include <unistd.h> //getopt() support

#include "console.h"
#include "config.h"

void usage(){
    printf("MiniC Simulator:\n\
Usage: simulator [options] execuation_file\n\
Options:\n\
\t-v verification mode\n\
\t-r auto run and exit to get final result\n\
\t-s silent mode, only output the final result\n\
\n");
}

int main(int argc, char **argv){
    CPU_d* cpu;
    int sig;
    int oc;
    while((oc = getopt(argc, argv, "vrs?")) != -1){
        switch(oc){
            case 'v':
                memset(pipline_output_invalid, 0, PIPLINE_LEVEL*sizeof(int));
                v_mode = 1;
                break;
            case 'r':
                auto_run_mode = 1;
                break;
            case 's':
                memset(pipline_output_invalid, 0, PIPLINE_LEVEL*sizeof(int));
                break;
            case '?':
                usage();
                return -1;
            default:
                usage();
                return -1;
        }
    }
    if(optind >= argc){
        usage();
        return -1;
    }
    //  initscr();
    //  refresh();
    //  echo();
    cpu = process_start(argv[optind]);
    //auto run mode
    if(auto_run_mode == 1){
        printf("MiniC Simulator Auto Run Mode:\n");
        console_run(cpu, argv[optind], 0);
        return 0;
    }
    // normal mode
    printf("MiniC Simulator begin.\n");
    printf("File : %s\n", argv[optind]);
    printf("CPU mode : normal\n");
    printf("=================================\n");
    while(1){
        sig = console_next_cmd(cpu, argv[optind]);
        if(sig == 0)
            break;
    }
    printf("MiniC Simulator end.\n");
    process_end(cpu);
    //  endwin();
    return 0;
}
