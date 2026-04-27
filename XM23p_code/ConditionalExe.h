//
// Created by Raymond St-Jean on 2024-07-30.
// This is the header file for the conditional execute instruction

#ifndef CONDITIONALEXE_H
#define CONDITIONALEXE_H

#include "PSW.h"
#include "RegisterFile.h"

extern unsigned int TFcase; //1 if True condition, 0 if false consition
extern unsigned int Tcount; //count of True condition instructions to execute
extern unsigned int Fcount; //count of False con inst to execute

//Numbered Condition codes
enum conCodes {EQ, NE, CS, CC, MI, PL, VS, VC, HI, LS, GE, LT, GT, LE, TR, FL};

//function that performs the conditional execute instruction
void ConExe(unsigned int Cond, unsigned char Tval, unsigned char Fval);

//CEX check for D0 stages, decides to bubble or not, and decrements the T/F count
void D0cex();

//exits the cex state by setting T and F count to 0
void clearCEX();
#endif //CONDITIONALEXE_H
