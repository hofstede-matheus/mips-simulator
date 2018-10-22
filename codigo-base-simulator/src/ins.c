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
    if(mem_read_32(CURRENT_STATE.REGS[2]) == 10){
        RUN_BIT = 0;
    }
}

int getType(uint32_t num){
    // 0 = tipo J
    // se OPCODE == 000010 ou 000011
    if(num == 2 || num == 3) return 0;
    // 1 = tipo I
    else if(getI(num)) return 1;
    // 2 = tipo R (ou seja, todas as outras)
    else return 2;
    return 0;
}

int getI(int code){
    // checa se o opcode recebido é um do tipo I
    int opcodes[] = {4, 5, 8, 9, 12, 13, 15, 35, 42};
    int i;
    for(i = 0; i < 9; i++){
        if(code == opcodes[i]) return 1;
    }
    return 0;
}