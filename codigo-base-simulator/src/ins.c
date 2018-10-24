#include <stdio.h>
#include <stdint.h>
#include "ins.h"
#include "shell.h"


// atualiza o PC
void next_instruction(){
    NEXT_STATE.PC+= 4;
}

//  retorna o tipo da instrução
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
// auxiliar pra identificar se é tipo I
int getI(int code){
    int opcodes[] = {4, 5, 8, 9, 12, 13, 15, 35, 42};
    int i;
    for(i = 0; i < 9; i++){
        if(code == opcodes[i]) return 1;
    }
    return 0;
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*                         COMUM À TODOS                        */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

// retorna o opcode da instrução
uint32_t getOPCODE(uint32_t num){
    num = num >> (32 - 6);
    return num;
}



/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*                      CAMPOS DO TIPO R                        */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


// retorna o RS da função
uint32_t getRS(uint32_t num){
    num = num >> (32 - 11);
    num = num << (32 - 5);
    num = num >> (32 - 5);
    return num;
}

// retorna o RT da função
uint32_t getRT(uint32_t num){
    num = num >> (32 - 16);
    num = num << (32 - 5);
    num = num >> (32 - 5);
    return num;
}

// retorna o RD da função
uint32_t getRD(uint32_t num){
    num = num >> (32 - 21);
    num = num << (32 - 5);
    num = num >> (32 - 5);
    return num;
}
uint32_t getSA(uint32_t num){
    num = num >> (32 - 26);
    num = num << (32 - 5);
    num = num >> (32 - 5);
    return num;
}
uint32_t getFUNC(uint32_t num){
    num = num << (32 - 6);
    num = num >> (32 - 6);
    return num;
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*                      CAMPOS DO TIPO J                        */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

uint32_t getADDRESS(uint32_t num){
    num = num << (32 - 26);
    num = num >> (32 - 26);
    return num;
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*                      CAMPOS DO TIPO I                        */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

//  GETRS E GETRT JÁ EXISTEM

uint32_t getIMMEDIATE(uint32_t num){
    num = num << (32 - 16);
    num = num >> (32 - 16);
    return num;
}





/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*              IMPLEMENTAÇÃO DAS INSTRUÇÕES                    */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

// implementa syscall
void SYSCALL(){
    if(mem_read_32(CURRENT_STATE.REGS[2]) == 10){
        RUN_BIT = 0;
    }
}