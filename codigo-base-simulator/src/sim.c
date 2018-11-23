#include <stdio.h>
#include "shell.h"
//#include "ins.h" modularização

// ESCOPO DAS FUNÇÕES

void next_instruction(); // PC+4

void select_instruction(uint32_t hex);
int getType(uint32_t num);
int getI(int code);
uint32_t getOPCODE(uint32_t num);

uint32_t getRS(uint32_t num);
uint32_t getRT(uint32_t num);
uint32_t getRD(uint32_t num);
uint32_t getSA(uint32_t num);
uint32_t getFUNC(uint32_t num);

uint32_t getADDRESS(uint32_t num);

uint32_t getIMMEDIATE(uint32_t num);


void SYSCALL();

void j(uint32_t hex);
void jal(uint32_t hex);

void addi(uint32_t num);
void addiu(uint32_t num);
void beq(uint32_t num);
void bne(uint32_t num);
void blez(uint32_t num);
void bgtz(uint32_t num);
void slti(uint32_t hex);
void sltiu(uint32_t hex);
void andi(uint32_t hex);
void ori(uint32_t hex);
void xori(uint32_t hex);
void lui(uint32_t hex);
void lb(uint32_t hex);
void lh(uint32_t hex);
void lw(uint32_t hex);
void lbu(uint32_t hex);
void lhu(uint32_t hex);
void sb(uint32_t hex);
void sh(uint32_t hex);
void sw(uint32_t hex);
void bltz(uint32_t hex);
void bgez(uint32_t hex);
void bltzal(uint32_t hex);
void bgezal(uint32_t hex);
void jr(uint32_t hex);
void jalr(uint32_t hex);

void add(uint32_t hex);
void addu(uint32_t hex);
void sub(uint32_t hex);
void subu(uint32_t hex);
void and(uint32_t hex);
void or(uint32_t hex);
void xor(uint32_t hex);
void nor(uint32_t hex);
void slt(uint32_t hex);
void sltu(uint32_t hex);
void mult(uint32_t hex);
void multu(uint32_t hex);
void mfhi(uint32_t hex);
void mflo(uint32_t hex);
void mthi(uint32_t hex);
void mtlo(uint32_t hex);
void div(uint32_t hex);
void divu(uint32_t hex);
void sll(uint32_t hex);
void srl(uint32_t hex);
void sra(uint32_t hex);
void sllv(uint32_t hex);
void srlv(uint32_t hex);
void srav(uint32_t hex);


void process_instruction();


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
            case 1:
                if(getRT(hex) == 0)bltz(hex);
                else if(getRT(hex) == 1)bgez(hex);
                else if(getRT(hex) == 17)bgezal(hex);
                else if(getRT(hex)== 16)bltzal(hex);
                break;
            case 4:
                beq(hex);
                break;
            case 5:
                bne(hex);
                break;
            case 6:
                blez(hex);
                break;
            case 7:
                bgtz(hex);
                break;
            case 8:
                addi(hex);
                break;
            case 9:
                addiu(hex);
                break;
            case 10:
                slti(hex);
                break;
            case 11:
                sltiu(hex);
                break;
            case 12:
                andi(hex);
                break;
            case 13:
                ori(hex);
                break;
            case 14:
                xori(hex);
                break;
            case 15:
                lui(hex);
                break;
            case 32:
                lb(hex);
                break;
            case 33:
                lh(hex);
                break;
            case 35:
                lw(hex);
                break;
            case 36:
                lbu(hex);
                break;
            case 37:
                lhu(hex);
                break;
            case 40:
                sb(hex);
                break;
            case 41:
                sh(hex);
                break;
            case 43:
                sw(hex);
                break;
        }
        
    }else{
        printf("!!!!!TIPO R!!!!!!\n");        
        switch(func){
            case 0:
                sll(hex);
                break;
            case 2:
                srl(hex);
                break;
            case 3:
                sra(hex);
                break;
            case 4:
                sllv(hex);
                break;
            case 6:
                srlv(hex);
                break;
            case 7:
                srav(hex);
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
    printf("!!!!!FORA!!!!!! %d   \n", CURRENT_STATE.REGS[2]);
    if(CURRENT_STATE.REGS[2] == 10){
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
    printf("1msb_PC!!!%x\n", msb_PC);
    msb_PC = msb_PC >> 28;
    msb_PC = msb_PC << 28;
    address = address << 2;
    printf("msb_PC!!!%x\n", msb_PC);
    printf("address!!!%x\n", address);
    uint32_t t = address | msb_PC;
    //t+= 4;
    printf("!!!%x\n", t);
    printf("%x\n", mem_read_32(t));
    NEXT_STATE.PC = t;

}
// JAL
void jal(uint32_t hex){
    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    j(hex);
}

/***************************************************************/
/*                                                             */
/*                     Instruções Tipo-I                       */
/*                     op|rs|rt|immediate                      */
/*                                                             */
/***************************************************************/

void beq(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);

    if(CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt]){
        NEXT_STATE.PC = NEXT_STATE.PC + (imm * 4) + 4;
        printf("%x", NEXT_STATE.PC);
    }
}

void bne(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);

    if(CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt]){
        NEXT_STATE.PC = NEXT_STATE.PC + (imm * 4) + 4;
        printf("%x", NEXT_STATE.PC);
    }
}

void blez(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);

    if(CURRENT_STATE.REGS[rs] <= 0){
        NEXT_STATE.PC = NEXT_STATE.PC + (imm * 4) + 4;
        printf("%x", NEXT_STATE.PC);
    }
}

void bgtz(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);

    if(CURRENT_STATE.REGS[rs] > 0){
        NEXT_STATE.PC = NEXT_STATE.PC + (imm * 4) + 4;
        printf("%x", NEXT_STATE.PC);
    }
}

void slti(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);

    if(CURRENT_STATE.REGS[rs] < imm){
        NEXT_STATE.REGS[rt] = 1;
    }else{
        NEXT_STATE.REGS[rt] = 0;
    }
}

void sltiu(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);

    if(CURRENT_STATE.REGS[rs] < imm){
        NEXT_STATE.REGS[rt] = 1;
    }else{
        NEXT_STATE.REGS[rt] = 0;
    }
}

void andi(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    rs = getRS(num);
    rt = getRT(num);
    

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    //rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    NEXT_STATE.REGS[rt] = rs & imm;
}

void ori(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    //rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    NEXT_STATE.REGS[rt] = rs | imm;
}
// XOR
void xori(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    //rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    NEXT_STATE.REGS[rt] = rs ^ imm;
}

void lui(uint32_t num){
    uint32_t imm, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    //rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    //rs = CURRENT_STATE.REGS[rs];
    //rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    NEXT_STATE.REGS[rt] = imm << 16;
}

void lb(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    //rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    
    uint32_t t = mem_read_32(rs + imm);
    t = t >> (32 - 8);
    NEXT_STATE.REGS[rt] = t;
}

void lh(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    //rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    
    uint32_t t = mem_read_32(rs + imm);
    t = t >> (32 - 16);
    NEXT_STATE.REGS[rt] = t;
}

void lw(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    //rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    
    uint32_t t = mem_read_32(rs + imm);
    NEXT_STATE.REGS[rt] = t;
}

void lbu(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    //rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    
    uint32_t t = mem_read_32(rs + imm);
    t = t >> (32 - 8);
    NEXT_STATE.REGS[rt] = t;
}

void lhu(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    //rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    
    uint32_t t = mem_read_32(rs + imm);
    t = t >> (32 - 16);
    NEXT_STATE.REGS[rt] = t;
}

void sb(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    
    rt = rt >> (32 - 8);
    mem_write_32(rs + imm, rt);
}

void sh(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    
    rt = rt >> (32 - 16);
    mem_write_32(rs + imm, rt);
}

void sw(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    imm = getIMMEDIATE(num);
    rs = getRS(num);
    rt = getRT(num);

    // ate agora eu tenho o numero do registrador
    // vamos pegar os valores desses registradores 
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    rt = CURRENT_STATE.REGS[rt];
    // agora tenho os valores de fato
    
    mem_write_32(rs + imm, rt);
}

void bltz(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);

    if(CURRENT_STATE.REGS[rs] < 0){
        NEXT_STATE.PC = NEXT_STATE.PC + (imm * 4) + 4;
        printf("%x", NEXT_STATE.PC);
    }
}

void bgez(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);

    if(CURRENT_STATE.REGS[rs] >= 0){
        NEXT_STATE.PC = NEXT_STATE.PC + (imm * 4) + 4;
        printf("%x", NEXT_STATE.PC);
    }
}

void bltzal(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);

    if(CURRENT_STATE.REGS[rs] < 0){
        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
        NEXT_STATE.PC = NEXT_STATE.PC + (imm * 4) + 4;
        printf("%x", NEXT_STATE.PC);
    }
}

void bgezal(uint32_t num){
    uint32_t imm, rs, rt; // numero do registrador
    rs = getRS(num);
    rt = getRT(num);
    imm = getIMMEDIATE(num);

    if(CURRENT_STATE.REGS[rs] >= 0){
        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
        NEXT_STATE.PC = NEXT_STATE.PC + (imm * 4) + 4;
        printf("%x", NEXT_STATE.PC);
    }
}


void jr(uint32_t num){
    uint32_t rs; // numero do registrador
    rs = getRS(num);

    rs = CURRENT_STATE.REGS[rs];

    NEXT_STATE.PC = rs;
}

void jalr(uint32_t num){
    uint32_t rs; // numero do registrador
    rs = getRS(num);

    rs = CURRENT_STATE.REGS[rs];
    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    NEXT_STATE.PC = rs;
}



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
    printf("%d\n", rs);
    rt = getRT(num);
    printf("%d\n", rt);
    imm = getIMMEDIATE(num);
    printf("%d\n", imm);
    //rd = CURRENT_STATE.REGS[rd];
    rs = CURRENT_STATE.REGS[rs];
    printf("%d\n", rs);
    //rt = CURRENT_STATE.REGS[rt];
    //mem_write_32()
    NEXT_STATE.REGS[rt] = rs + imm;
    printf("%d kkkk", mem_read_32(CURRENT_STATE.REGS[rt]));
    printf("%d kkkk", mem_read_32(NEXT_STATE.REGS[rt]));
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

void sll(uint32_t num){
    uint32_t amm, rd, rt; // numero do registrador
    rd = getRD(num);
    rt = getRT(num);
    amm = getSA(num);

    rt = CURRENT_STATE.REGS[rt];

    NEXT_STATE.REGS[rd] = rt << amm;
}

void srl(uint32_t num){
    uint32_t amm, rd, rt; // numero do registrador
    rd = getRD(num);
    rt = getRT(num);
    amm = getSA(num);

    rt = CURRENT_STATE.REGS[rt];

    NEXT_STATE.REGS[rd] = rt >> amm;
}

void sra(uint32_t num){
    uint32_t amm, rd, rt; // numero do registrador
    rd = getRD(num);
    rt = getRT(num);
    amm = getSA(num);

    rt = CURRENT_STATE.REGS[rt];

    uint32_t signal;

    signal = rt >> 31;
    signal = signal << 31;

    rt = (rt >> amm) | signal;
    NEXT_STATE.REGS[rd] = rt;
}

void sllv(uint32_t num){
    uint32_t rs, rd, rt; // numero do registrador
    rd = getRD(num);
    rt = getRT(num);
    rs = getRS(num);

    rt = CURRENT_STATE.REGS[rt];
    rs = CURRENT_STATE.REGS[rs];


    NEXT_STATE.REGS[rd] = rt >> rs;
}

void srlv(uint32_t num){
    uint32_t rs, rd, rt; // numero do registrador
    rd = getRD(num);
    rt = getRT(num);
    rs = getRS(num);

    rt = CURRENT_STATE.REGS[rt];
    rs = CURRENT_STATE.REGS[rs];


    NEXT_STATE.REGS[rd] = rt << rs;
}

void srav(uint32_t num){
    uint32_t rs, rd, rt; // numero do registrador
    rd = getRD(num);
    rt = getRT(num);
    rs = getRS(num);

    rt = CURRENT_STATE.REGS[rt];
    rs = CURRENT_STATE.REGS[rs];

    uint32_t signal;

    signal = rt >> 31;
    signal = signal << 31;

    rt = (rt >> rs) | signal;
    NEXT_STATE.REGS[rd] = rt;
}


void process_instruction()
{
    printf("----------------------------------\n");
    printf("PC ADDRESS: %x\n", CURRENT_STATE.PC);
    printf("PC HEX: %x\n", mem_read_32(CURRENT_STATE.PC));
    printf("\n\n");
    printf("TYPE: ");
    printf("%u\n", getType(getOPCODE(mem_read_32(CURRENT_STATE.PC))));
    printf("OPCODE: ");
    printf("%u\n", getOPCODE(mem_read_32(CURRENT_STATE.PC)));
    if(getType(getOPCODE(mem_read_32(CURRENT_STATE.PC))) == 2){
        printf("é tipo R: \n");
        printf("RS: ");
        printf("%u\n", getRS(mem_read_32(CURRENT_STATE.PC)));
        printf("RT: ");
        printf("%u\n", getRT(mem_read_32(CURRENT_STATE.PC)));
        printf("RD: ");
        printf("%u\n", getRD(mem_read_32(CURRENT_STATE.PC)));
        printf("SA: ");
        printf("%u\n", getSA(mem_read_32(CURRENT_STATE.PC)));
        printf("FUNC: ");
        printf("%u\n", getFUNC(mem_read_32(CURRENT_STATE.PC)));
    }
    if(getType(getOPCODE(mem_read_32(CURRENT_STATE.PC))) == 0){
        printf("é tipo J: \n");
        printf("ADDRESS: ");
        printf("%x\n", getADDRESS(mem_read_32(CURRENT_STATE.PC)));
        //printf("%x\n", mem_read_32(getADDRESS(mem_read_32(CURRENT_STATE.PC))));
        //printf("%x\n", mem_read_32((uint32_t) 1048578 ));

    }
    if(getType(getOPCODE(mem_read_32(CURRENT_STATE.PC))) == 1){
        printf("é tipo I: \n");
        printf("RS: ");
        printf("%u\n", getRS(mem_read_32(CURRENT_STATE.PC)));
        printf("RT: ");
        printf("%u\n", getRT(mem_read_32(CURRENT_STATE.PC)));
        printf("IMMEDIATE: ");
        printf("%u\n", getIMMEDIATE(mem_read_32(CURRENT_STATE.PC)));
        printf("%x\n", mem_read_32(getIMMEDIATE(mem_read_32(CURRENT_STATE.PC))));

    }
    select_instruction(mem_read_32(CURRENT_STATE.PC));
    printf("----------------------------------\n");

    next_instruction();


    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */




    
}



