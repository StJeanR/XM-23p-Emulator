//
// Created by raymond st-jean on 2024-06-21.
//Header file for the executing instrunctions

#ifndef INSTRUCTIONEXECUTER_H
#define INSTRUCTIONEXECUTER_H

#include "PSW.h"
#include "Fetcher.h"
#include "RegisterFile.h"
#include "StageE1.h"
#include "ConditionalExe.h"


//macros for accessing instruction structures
#define dest toExe.REGS.DST     //Destination
#define rSource toExe.REGS.SC   //Source
#define rBW toExe.REGS.WB       //Byte or Word
#define rRC toExe.REGS.RC       //Register or Constant
#define mByte toExe.MOVS.byte   //byte to move for MOV
#define ClrSt toExe.PSW.pswCC   //bits for clearing or setting PSW
#define PrPo toExe.LDST.PRPO    //Pre or Post for LD/ST
#define LSinc toExe.LDST.INC    //Increment for Ld/St
#define LSdec toExe.LDST.DEC    //Decrement for Ld/St
//set the low or high byte of a register to a value
#define setByte(regNum, HiLo, newVal) regByte(r, regNum, HiLo) = newVal
//clear byte and set byte value
#define ByteClear 0x00
#define ByteSet 0xFF
//word or byte bit definitions
#define w 0
#define b 1

//instruction to execute
extern Reg toExe;

//Structure for Pre or Post inc/dec in Memory Accessing instructions
typedef struct Pre_Po {
    short postpre[biArray]; //element 0 for post, element 1 for pre
}Pre_Po;

//executes the previously decoded instruction
void E0();

//takes a 10 bit offset as input, left shifts it, sign extend the OFFset
void branch10Bit(short OFF);
//change the pc to branch to a new location
void brWithOffset(short OFF);
//perform an add with carry, carry and source value is modified for use with ADD ADDC SUB SUBC
void AddWithCarry(int srcVal, int dst, int carry);
//adds nibbles, converting the result into a decimal value with carry if larger than 9
char nibbleAdd(char* result, unsigned char Ndst, unsigned char Nsrc);

//shifts a register right by 1, and returns the carry
char shiftRight();
//returns the bit offset from source in either word or byte length
short bitOffset();
//functions for memory access instructions
//LD and ST fucntion
void LoadStore(unsigned int accessReg);
//Send address to access by LD ST LDR STR, plus offset
void sendDMAR(short address, char offset);

//check for PSW bits
void checkZ(short dst);
void checkCV(int dstSign, int srcSign, int resultSign);
void checkN(short dst);

#endif //INSTRUCTIONEXECUTER_H
