#ifndef _SIM_INS_H_
#define _SIM_INS_H_



void next_instruction();
int getType(uint32_t num);
int getI(int code);
uint32_t getOPCODE(uint32_t num);
uint32_t getRS(uint32_t num);
uint32_t getRT(uint32_t num);
uint32_t getRD(uint32_t num);
uint32_t getSA(uint32_t num);
uint32_t getFUNC(uint32_t num);


void SYSCALL();


#endif