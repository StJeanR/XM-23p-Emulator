//Raymond St-Jean
//ECED 3403
//Memory_Loader.h
//This is the header which contains the global instances of memory and the functions for loading

#ifndef MEMORY_LOADER_H
#define MEMORY_LOADER_H

#include <stdio.h>
#include "Decoder_D0.h"
#include "FileLoad.h"
#include "RegisterFile.h"
#include "Clock.h"
#include "Fetcher.h"

#define inputSize 15
//size of IMEM and DMEM is 64 KiB
#define byteMemSize 65536
#define wordMemSize 32768 // 65536/2
//Define the memory types
#define InstIndex 0
#define DataIndex 1
#define FileNameLen 51 //max length of filename for input
#define mul16 4    //number of left shifts that multiplies an int by 16

#define optionMessage "\nMain Menu\nl\t-Load File\nmi\t-View Instrucion Memory\nmd\t-View Data Memory"\
"\n\tformat: m_ xxxx xxxx\nD\t-Decode\ng\t-go\nr\t-change registers or memory\n"\
"b\t-New Break Address\nR\t-Reset (load a new program to run first)\nx\t-Exit\nOption: \n"

//union to access program memory using byte and/or word addresses
union InstDatMem {
    //word addresses
    unsigned short words[wordMemSize];
    //byte addresses
    unsigned char bytes[byteMemSize];
};

typedef struct ProgramMemory {
    //Instruction and Data memory. 0 is instruction, 1 is data
    union InstDatMem MEM[2];

    //Name of source file
    char asmFileName[FileNameLen];
    //address of first instruction
    int startAddress;
} ProgramMemory;

extern ProgramMemory PM;

//Function for user input to view memory or load more programs
void memAccess();
//void loadMEM(char memType, int address, char byte, short word);
void viewMEM(int start, int end, int mType);
//Hex and Dec conversion function
int HexToDec (char hex);

#endif //MEMORY_LOADER_H