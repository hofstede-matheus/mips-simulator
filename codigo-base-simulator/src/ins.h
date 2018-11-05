#ifndef _SIM_INS_H_
#define _SIM_INS_H_



void next_instruction();

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



#endif