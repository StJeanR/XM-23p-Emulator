//
// Created by Ratmond St-Jean on 2024-06-05.
//The header file for the RegisterFile, which have variables for R0 through R7 and the constant values

#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "Decoder_D0.h"
#include "Memory_Loader.h"
#include "Fetcher.h"
#include "stdio.h"

#define numRegisters 8
#define biArray 2
#define hiByte 1
#define loByte 0
#define r 0
#define c 1
#define one 1
#define BrkPntOffset 2

//access, or change register contents
#define regWord(rc, n) (ProgramRegisters.regConArray[rc][n].word)
#define regByte(rc, n, b) (ProgramRegisters.regConArray[rc][n].bytes[b])
#define regNibble(rc, n, NIB) (ProgramRegisters.regConArray[rc][n].nibbles.NIB)
//sets a register to a given value, used by executor
#define setRegWord(newVal, regNum) (ProgramRegisters.regConArray[r][regNum].word = newVal)
//check a bit in a varaible
#define checkBIT(var,pos) (((var) & (1<<(pos)))>>pos)


extern struct regcons ProgramRegisters;

//uses Register union as declared in Fetcher.c
struct regcons {
    //array to store registers
    union reg regConArray[biArray][numRegisters];
    //array of constant values
};



//debugger runs the below debugger functions
void debugger();
//display the contents of a register
void displayRegister();
void changeRegister();
void changeMemory(int memType);
void setBreakAddress();

short getHexWord();
void initRegs();


#endif //DEBUGGER_H
