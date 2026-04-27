//Raymond St-Jean
//ECED 3403
//Memory_Loader.h
//This file contains the functions for loading into memory for the XM23p emulator

#include "Memory_Loader.h"

#include "Fetcher.h"

#define outputLineLen 16
#define sizeHex 16
#define Ahex 10
#define nul 0
#define ByteWordShift 1

ProgramMemory PM;

//File finished reading, take input from user to view memory, or load another program
void memAccess() {
    char input[inputSize] = {0};
    //Give option to user to view memory
    printf(optionMessage);
    fscanf(stdin, " %15[^\n]", input);
    getchar();
    int i = 0;

    while(input[i] != 'x') {
        switch (input[i]) {
            case 'm':
                i++;
            int addr1=0, addr2=0;
            int memType = input[i];

            //move to first address
            while(input[i] == ' ' || input[i] == 'i' || input[i] == 'd') {
                i++;
            }
            //read starting address
            while(input[i] != ' ') {
                addr1 = addr1 << mul16;
                addr1 += HexToDec(input[i]);
                i++;
            }
            while(input[i] == ' ') i++;     //move to second address

            //read ending address
            while(input[i] != nul && input[i] != ' ') {
                addr2 = addr2 << mul16;
                addr2 += HexToDec(input[i]);
                i++;
            }

            if(memType == 'i') {
                //access instruction memory
                viewMEM(addr1, addr2, InstIndex);
            }
            else if(memType == 'd') {
                //access data memory
                viewMEM(addr1, addr2, DataIndex);
            }
            break;

            case 'l':
                Load();
            break;

            case 'D':
                //decode
                decode();
            break;

            case 'g':
                //Begin similation
                go();
            break;

            case 'r':
                //launch debugger
                    debugger();
                    printf(optionMessage); //reprint the options after returning
            break;

            case 'R':
                //reset to run newly loaded file
                resetClk();
            break;
            
            case 'b':
                setBreakAddress();
            break;

            default:
                printf("invalid input \n");
        }

        printf("Option: \n");
        fscanf(stdin, " %15[^\n]", input);
        getchar();
        i=0;
    }
}



void viewDMEM(int start, int end) {
    int roundStart = start - (start % outputLineLen);

    while(roundStart < end) {
        printf("%04x: ", roundStart);
        for(int i=0; i < outputLineLen; i++) {
            //printf("%02x ", PM.MEM[]);
        }
        roundStart = roundStart+outputLineLen;
        printf("\n");
    }
}

//print memory addresses from given start to given end addresses
void viewMEM(int start, int end, int mType) {
    //Need to convert to word address
    //int Wend = end << ByteWordShift;
    int roundStart = start - (start % outputLineLen);
    //roundStart = roundStart << ByteWordShift;

    while(roundStart < end) {
        printf("%04x:", roundStart);
        for(int i=0; i < outputLineLen; i++) {
            printf("%02x ", PM.MEM[mType].bytes[roundStart+i]);
        }
        roundStart = roundStart+outputLineLen;
        printf("\n");
    }
}

//converts char hexadecimal to int value
int HexToDec (char hex)
{
    if(hex >= '0' && hex <= '9')
        return hex - '0';
    if(hex >= 'A' && hex <= 'F')
        return hex - 'A'+Ahex;
    if(hex >= 'a' && hex <= 'f')
        return hex - 'a'+Ahex;
    return -1;
}