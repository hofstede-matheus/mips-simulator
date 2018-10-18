#ifndef _SIM_INS_H_
#define _SIM_INS_H_



void next_instruction();
uint32_t getOPCODE(uint32_t num);
uint32_t getRS(uint32_t num);
int getType(uint32_t num);
int getI(int code);
#endif