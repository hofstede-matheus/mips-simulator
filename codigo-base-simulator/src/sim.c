#include <stdio.h>
#include "shell.h"
#include "ins.h"

void process_instruction()
{
    //printf("%d\n", RUN_BIT);
    //NEXT_STATE.PC = 000000;
    
    next_instruction();

    //RUN_BIT = 0;
    //u32 trimmed = value & 0x3F;
    //printf("%b\n", trimmed);

    //int k = 604110858 >> 26;
    //k = k & 12;
    //int k = 604110858 << 5;
    //k = k >> 26;
    printf("----------------------------------\n");
    printf("%u\n", CURRENT_STATE.PC);
    printf("%x\n", mem_read_32(CURRENT_STATE.PC));
    printf("\n\n");
    printf("TYPE: ");
    printf("%u\n", getType(getOPCODE(mem_read_32(CURRENT_STATE.PC))));
    printf("----------------------------------\n");




    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
}


