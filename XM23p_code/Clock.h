//
// Created by Raymond St-Jean on 2024-06-25.
//Header for the clock cycle of Emulator

#ifndef CLOCK_H
#define CLOCK_H

#include <signal.h> //Signal handling software

#define TRUE 1
#define FALSE 0
//if debug is 1, debug messages are printed while running
#define debug 1
#define verbose 1 //if verbose is 1, more detailed messages are printed with debug

#define DmessageSize 51
#define noopInstr 0x4C00 //MOV R0,R0

extern char Dmessage[DmessageSize];
extern char E1message[DmessageSize];

extern char BubbleBit;
extern volatile sig_atomic_t ctrl_c_fnd;

//run program instructions
void go();
//perform a clock cycle
void clock();
//reset relevant variables to run a new program
void resetClk();
//print information for clock cycle, if debugging
void printCycle();
//handler for ^C interrupts
void sigint_hdlr();
#endif //CLOCK_H
