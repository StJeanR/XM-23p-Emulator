//
// Created by Raymond St-Jean on 2024-06-23.
//Header for the Program Status Word
//used for the status register and status bits

#ifndef PSW_H
#define PSW_H

#include <stdio.h>

#define clr 0
#define set 1
#define bit 1
#define biArray 2
#define PSWsize 5

#define sign(val) (checkBIT(val, msbTable[rBW]))

//Status register in the PSW that keeps track of important qualities of previous instruction
struct StatusRegBits{
        unsigned int carry:bit;         //set if previous result had a carry
        unsigned int zero:bit;      //set if prev result was negative
        unsigned int negative:bit;  //set if previous inst result was 0
        unsigned int sleep:bit;   //set if next instruction should not execute
        unsigned int overflow:bit;      //set if previous inst had an overflow
};

struct StatusRegPSW {
        unsigned int psw:PSWsize;
};

union statusReg {
        struct StatusRegBits bits;  //PSW bits separate
        struct StatusRegPSW comb; //PSW bits combined as int
};

extern union statusReg PSW;

extern unsigned int overflowTable[biArray][biArray][biArray];
extern unsigned int carryTable[biArray][biArray][biArray];

void setC();
void clearC();
void setN(int newN);
void clearN();
void setZ();
void clearZ();
void setV();
void clearV();

//or the PSW bits with an input. Sets the bits that are set in the input
void OrPSW(int toSet);  //used with SETCC
//Toggle bits in the PSW
void togglePSW(int toTog);      //used with CLRCC
//display the PSW bits
void displayPSW();

#endif //PSW_H
