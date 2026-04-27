//
// Created by rstjean on 2024-06-05.
//Functions that are used to view and change register values, change memory values, and set a breakpoint
//


#include "RegisterFile.h"

#include "Decoder_D0.h"
#include "Memory_Loader.h"
#include "Fetcher.h"

#define wordHex 4
#define constantValues {0, 1, 2, 4, 8, 16, 32, -1}

#define changeMessage "Welcome to the Debugger\n\nrd\t-Display Register\nrc\t-Change Register Value\n"\
           "m_\t-Change Memory at Address in instruction (_i) or data (_d) memory\n"\
           "b\t-Set Break Address\np\t-Display PSW\nx\t-Return\nOption: \n"

struct regcons ProgramRegisters;

void debugger() {
    //get input from the user to see what to run
    char input[inputSize] = {0};
    //Give option to user to view memory
    printf(changeMessage);
    fscanf(stdin, " %15[^\n]", input);
    getchar();
    int i = 0;
    //get user input
    while(input[i] != 'x') {
        switch (input[i]) {
            case 'r':
                i++;
                if (input[i] == 'd') {
                    //view/display register
                    displayRegister();
                }
                else if(input[i] == 'c') {
                    //change register
                    changeRegister();
                }
                else {
                    printf("error: r should be followed by d (display) or c (change)");
                }
            break;

            case 'm':
                i++;
                if(input[i] == 'i') {
                    //change instr memory
                    changeMemory(InstIndex);
                }
                else if(input[i] == 'd') {
                    //change data mem
                    changeMemory(DataIndex);
                }
                else {
                    printf("error: m should be followed by 'i' (instruction) or 'd' (data)");
                }
            break;

            case 'b':
                setBreakAddress();
            break;
            case 'p':
                displayPSW();
            break;

            default:
                printf("invalid input \n");
            break;
        }
        printf("Option: \n");
        fscanf(stdin, " %15[^\n]", input);
        getchar();
        i = 0;
    }
}

void displayRegister() {
    //loop through all registers
    for(int regNum = 0; regNum < numRegisters; regNum++) {
        //print reg contents
        printf("R%d: %04x\n", regNum, ProgramRegisters.regConArray[r][regNum].word);
    }

}

void changeRegister() {
    printf("Provide register number to change (0-7):\n");
    //get Register number from user
    int regNum = getchar() - '0';
    //get the new value from user
    printf("Provide update as Hexadecimal word:\n");
    if(regNum == pc) printf("warning, PC/reg7 uses word addressing\n");
    short newVal = getHexWord();
    //Update value stored in register
    ProgramRegisters.regConArray[r][regNum].word = newVal;
}
void changeMemory(int memType) {
    //get the address from user
    printf("Provide address as Hexadecimal word:\n");
    short addr = getHexWord();
    //get the new value from user
    printf("Provide Hexadecimal word update:\n");
    short newVal = getHexWord();
    //Store the new value at specified address
    PM.MEM[memType].words[addr >> one] = newVal;
}
void setBreakAddress() {
    //get new break address from user
    printf("Provide new break address as Hexadecimal word:\n");
    short newVal = getHexWord() + BrkPntOffset;
    //set new break address
    endAddress = newVal >> one;
}

short getHexWord() {
    char input[wordHex];
    //get new break address from user
    int buffer = 0;
    short HexVal;
    fscanf(stdin, " %04[^\n]", input);
    sscanf_s(input, "%04x\n", &buffer);
    HexVal = buffer;

    return HexVal;
}
void initRegs() {
    int consts[numRegisters] = constantValues;
    for(int i = 0; i< numRegisters; i++) {
        //ProgramRegisters.regConArray[r][i].word = 0;
        ProgramRegisters.regConArray[c][i].word = consts[i];
    }
}



