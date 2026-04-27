//
// Created by Raymond St-Jean on 2024-05-2
//Fetching instructions for ECED 3403Lab
//



#include "Fetcher.h"

#include "FileLoad.h"
#include "Memory_Loader.h"
#include "RegisterFile.h"

union reg IMAR;
union reg IMBR;
union reg IR;

int clk;
//unsigned short PC;


void initFetch() {
    //PC gets starting address

    ProgramRegisters.regConArray[r][pc].word = PM.startAddress >> 1;
    //convert PC to word address
    //  ProgramRegisters.regConArray[r][pc] = ProgramRegisters.regConArray[r][pc] >> 1;
    printf("\n%04x\n", ProgramRegisters.regConArray[r][pc].word);
    clk = 0;

    //ICTRL is set to signify no word is to read

    //Set IR to perform a No Op (like MOV r1, r1)
}

//fetch the next instruction
union reg iFetch() {
    IMAR = F0();
    IR = F1();
    return IR;
}

//get next address to read
union reg F0() {
    union reg nextInstAddr;
    nextInstAddr.word = ProgramRegisters.regConArray[r][pc].word;
    ProgramRegisters.regConArray[r][pc].word = ProgramRegisters.regConArray[r][pc].word + pcIncrease;
    return nextInstAddr;
    //set ICTRL to read a word during F1
}

union reg F1() {
    //load instruction at address in IMAR into IMBR
    IMBR.word = PM.MEM[InstIndex].words[IMAR.word];
    return IMBR;
}