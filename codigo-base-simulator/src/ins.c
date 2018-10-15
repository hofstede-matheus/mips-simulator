#include <stdio.h>
#include <stdint.h>
#include "ins.h"
#include "shell.h"




uint32_t getOPCODE(uint32_t num){
    num = num >> (32 - 6);
    return num;
}

uint32_t getRS(uint32_t num){
    num = num >> (32 - 11);
    num = num << (32 - 5);
    num = num >> (32 - 5);
    return num;
}

void next_instruction(){
    NEXT_STATE.PC+= 4;
}

void SYSCALL(){
    RUN_BIT = 0;
}

