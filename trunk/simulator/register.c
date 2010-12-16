#include <unistd.h>
#include <malloc.h>
#include "register.h"


//initial function
REGISTERS * reg_initial(){
    REGISTERS* regs = malloc(sizeof(REGISTERS));
    int i;
    for(i=0; i<32; i++){
        regs->r[i] = 0;
        regs->flag = 0;
    }
    return regs;
}

void reg_destroy(REGISTERS *regs){
    free(regs);
}


//flag operations
//flag N operation
int reg_setN(REGISTERS *regs, int n){
    if(regs == NULL)
        return 0;
    if(n!=0)
        n = 1;
    if(n == 1)
        regs->flag = regs->flag | (1<<31);
    else
        regs->flag = regs->flag & ~(1<<31);
    return 1;
}

int reg_getN(REGISTERS *regs){
    if(regs == NULL)
        return 2;
    return (regs->flag)>>31 & 1;
}

//flag Z operation
int reg_setZ(REGISTERS *regs, int z){
    if(regs == NULL)
        return 0;
    if(z!=0)
        z = 1;
    if(z == 1)
        regs->flag = regs->flag | (1<<30);
    else
        regs->flag = regs->flag & ~(1<<30);
    return 1;
}

int reg_getZ(REGISTERS *regs){
    if(regs == NULL)
        return 2;
    return (regs->flag)>>30 & 1;
}

//flag C operation
int reg_setC(REGISTERS *regs, int c){
    if(regs == NULL)
        return 0;
    if(c!=0)
        c = 1;
    if(c == 1)
        regs->flag = regs->flag | (1<<29);
    else
        regs->flag = regs->flag & ~(1<<29);
    return 1;
}

int reg_getC(REGISTERS *regs){
    if(regs == NULL)
        return 2;
    return (regs->flag)>>29 & 1;
}

//flag V operation
int reg_setV(REGISTERS *regs, int v){
    if(regs == NULL)
        return 0;
    if(v!=0)
        v = 1;
    if(v == 1)
        regs->flag = regs->flag | (1<<28);
    else
        regs->flag = regs->flag | ~(1<<28);
    return 1;
}

int reg_getV(REGISTERS *regs){
    if(regs == NULL)
        return 2;
    return (regs->flag)>>28 & 1;
}
