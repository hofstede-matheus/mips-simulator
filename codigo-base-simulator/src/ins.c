#include <stdio.h>
#include <stdint.h>
#include "ins.h"
#include "shell.h"


// atualiza o PC
void next_instruction(){
    NEXT_STATE.PC+= 4;
}

// escolhe a instrução apropriada
void select_instruction(uint32_t hex){
    uint32_t type, opcode, func;
    func = getFUNC(hex);
    opcode = getOPCODE(hex);

    type = getType(opcode);

    if(type == 0){
        printf("!!!!!TIPO J!!!!!!\n");
        switch(opcode){
            case 2:
                j(hex);
                break;
            case 3:
                jal(hex);
                break;
        }
    }else if(type == 1){
        printf("!!!!!TIPO i!!!!!!\n");
        switch(opcode){
            case 8:
                addi(hex);
                break;
            case 9:
                addiu(hex);
                break;
        }
        
    }else{
        printf("!!!!!TIPO R!!!!!!\n");        
        switch(func){
            case 12:
                SYSCALL();
                break;
            case 24:
                mult(hex);
                break;
            case 32:
                add(hex);
                break;
            case 33:
                addu(hex);
                break;
            case 34:
                sub(hex);
                break;
            case 35:
                subu(hex);
                break;
            case 36:
                and(hex);
                break;
            case 37:
                or(hex);
                break;
            case 38:
                xor(hex);
                break;
            case 39:
                nor(hex);
                break;
            case 42:
                slt(hex);
                break;
            case 43:
                sltu(hex);
                break;
        }
    }
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
        printf("!!!!!MASSA!!!!!!\n");
        RUN_BIT = 0;
    }
}
/***************************************************************/
/*                                                             */
/*                     Instruções Tipo-J                       */
/*                         op|target                           */
/*                                                             */
/***************************************************************/

// J
void j(uint32_t hex){
    uint32_t address;
    address = getADDRESS(hex);
    uint32_t msb_PC = CURRENT_STATE.PC;
    msb_PC >> 28;
    msb_PC << 28;
    address << 2;
    uint32_t t = address | msb_PC;
    t+= 4;
    printf("!!!%x\n", t);
    printf("%x\n", mem_read_32(t));
    //NEXT_STATE.PC = t;

}
// JAL
void jal(uint32_t hex){
    uint32_t address;
    address = getADDRESS(hex);
}

/***************************************************************/
/*                                                             */
/*                     Instruções Tipo-I                       */
/*                     op|rs|rt|immediate                      */
/*                                                             */
/***************************************************************/

// ADDI

void addi(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);
    

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    //rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    if((rs + imm) > 2147483647){ // checa se vai dar overflow e joga a exception
        printf("OVERFLOW EXCEPTION");
    }else{
        NEXT_STATE.REGS[rt] = rs + imm;
    }
}
// ADDIU
void addiu(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);

    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    //rt = CURRENT_STATE.REGS[rt];
    NEXT_STATE.REGS[rt] = rs + imm;
}



/***************************************************************/
/*                                                             */
/*                     Instruções Tipo-R                       */
/*                  op|rs|rt|rd|shamt|funct                    */
/*                                                             */
/***************************************************************/

// ADD
void add(uint32_t num){
    uint32_t rd, rs, rt; // numero do registrador
    rd = getRD(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    if((rs + rt) > 2147483647){ // checa se vai dar overflow e joga a exception
        printf("OVERFLOW EXCEPTION");
    }else{
        NEXT_STATE.REGS[rd] = rs + rt;
    }
}
// ADDU
void addu(uint32_t num){
    uint32_t rd, rs, rt; // numero do registrador
    rd = getRD(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    NEXT_STATE.REGS[rd] = rs + rt;
}
// SUB
void sub(uint32_t num){
    uint32_t rd, rs, rt; // numero do registrador
    rd = getRD(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    if((rs - rt) < -2147483648){ // checa se vai dar overflow e joga a exception
        printf("OVERFLOW EXCEPTION");
    }else{
        NEXT_STATE.REGS[rd] = rs - rt;
    }
}
// SUBU
void subu(uint32_t num){
    uint32_t rd, rs, rt; // numero do registrador
    rd = getRD(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    NEXT_STATE.REGS[rd] = rs - rt;
}
// AND
void and(uint32_t num){
    uint32_t rd, rs, rt; // numero do registrador
    rd = getRD(num);
    rs = getRS(num);
    rt = getRT(num);
    

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    NEXT_STATE.REGS[rd] = rs & rt;
}
// OR
void or(uint32_t num){
    uint32_t rd, rs, rt; // numero do registrador
    rd = getRD(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    NEXT_STATE.REGS[rd] = rs | rt;
}
// XOR
void xor(uint32_t num){
    uint32_t rd, rs, rt; // numero do registrador
    rd = getRD(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    NEXT_STATE.REGS[rd] = rs ^ rt;
}
// NOR
void nor(uint32_t num){
    uint32_t rd, rs, rt; // numero do registrador
    rd = getRD(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    NEXT_STATE.REGS[rd] = ~(rs | rt);
}
// SLT
void slt(uint32_t num){
    uint32_t rd, rs, rt; // numero do registrador
    rd = getRD(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    if(rs < rt){
        NEXT_STATE.REGS[rd] = 1;
    }else{
        NEXT_STATE.REGS[rd] = 0;
    }
    
}
// SLTU
void sltu(uint32_t num){
    uint32_t rd, rs, rt; // numero do registrador
    rd = getRD(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    if(rs < rt){
        NEXT_STATE.REGS[rd] = 1;
    }else{
        NEXT_STATE.REGS[rd] = 0;
    }
    
}

// MULT
void mult(uint32_t num){
    printf("!!!!!TEI!!!!!!\n");
    uint32_t rs, rt, hi, lo; // numero do registrador
    int64_t temp;
    rs = getRS(num);
    rt = getRT(num);

    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    printf("%d\n", rs);
    printf("%d\n", rt);

    /*
    temp = (int64_t)(int32_t)rs * (int64_t)(int32_t)rt;
    hi = (uint32_t)((temp>>32) & 0xFFFFFFFF);
    lo = (uint32_t)(temp & 0xFFFFFFFF);
    */
    temp = rs * rt;
    printf("%ld\n", temp);
    hi = (uint32_t)(temp>>32);
    temp = temp << 32;
    temp = temp >> 32;
    lo = (uint32_t)temp;

    NEXT_STATE.HI = hi;
    NEXT_STATE.LO = lo;    
}
void multu(uint32_t num){
    printf("!!!!!TEI!!!!!!\n");
    uint32_t rs, rt, hi, lo; // numero do registrador
    int64_t temp;
    rs = getRS(num);
    rt = getRT(num);

    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    printf("%d\n", rs);
    printf("%d\n", rt);

    /*
    temp = (int64_t)(int32_t)rs * (int64_t)(int32_t)rt;
    hi = (uint32_t)((temp>>32) & 0xFFFFFFFF);
    lo = (uint32_t)(temp & 0xFFFFFFFF);
    */
    temp = rs * rt;
    printf("%ld\n", temp);
    hi = (uint32_t)(temp>>32);
    temp = temp << 32;
    temp = temp >> 32;
    lo = (uint32_t)temp;

    NEXT_STATE.HI = hi;
    NEXT_STATE.LO = lo;    
}
void mfhi(uint32_t hex){
    uint32_t rd; // numero do registrador
    rd = getRD(hex);
    NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
}
void mflo(uint32_t hex){
    uint32_t rd; // numero do registrador
    rd = getRD(hex);
    NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
}
void mthi(uint32_t hex){
    uint32_t rs; // numero do registrador
    rs = getRS(hex);
    NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
}
void mtlo(uint32_t hex){
    uint32_t rs; // numero do registrador
    rs = getRS(hex);
    NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
}
void div(uint32_t hex){
    uint32_t rs, rt;
    rs = getRS(hex);
    rt = getRT(hex);

    NEXT_STATE.HI = rs % rt;
    NEXT_STATE.LO = rs / rt;
}
void divu(uint32_t hex){
    uint32_t rs, rt;
    rs = getRS(hex);
    rt = getRT(hex);

    NEXT_STATE.HI = rs % rt;
    NEXT_STATE.LO = rs / rt;
}
