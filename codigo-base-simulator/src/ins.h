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

void add(uint32_t hex);


#endif