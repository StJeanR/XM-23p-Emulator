//
// Created by Raymond St-Jean on 2024-05-29.
//  Functions for the decoder of the emulator
//  determines the opcode of an instructtion, and the registers/constants used


#include "Decoder_D0.h"
#include "RegisterFile.h"

#define inputSiz 2

enum opCodes nextInstrType;
short endAddress;

#define normRegs 12
#define CLScode 15 //rType code is larger than 15 for CEX LD and ST
#define branches 8
#define normRegs2 2
#define normRegs3 5
#define cpuInst 6
#define CLS 4
#define movInst 4
//tables for opcodes
char* BLch = "BL";
char* aBranch[branches] = {"BEQ/BZ", "BNE/BNZ", "BC/BHS", "BNC/BLO", "BN", "BGE", "BLT", "BRA"};
char* aRegs1[normRegs] = {"ADD", "ADDC", "SUB", "SUBC",
    "DADD", "CMP", "XOR", "AND", "OR", "BIT", "BIC", "BIS"};
char* aRegs2[normRegs2] = {"MOV", "SWAP"};
char* aRegs3[normRegs3] = {"SRA", "RRC", "INVALID", "SWPB", "SXT"};
char* cpuStr[cpuInst] = {"SETPRI", "SVC", "SETCC", "SETCC", "CLRCC", "CLRCC"};
char* aCexLdSt[CLS] = {"CEX", "INVALID", "LD", "ST"};
char* aMOVx[movInst]= {"MOVL", "MOVLZ", "MOVLS", "MOVH"};

//tables for assigning opCode for executer
enum opCodes eBranches[branches] = {BEQ, BNE, BC, BNC, BN, BGE, BLT, BRA};
enum opCodes eRegs1[normRegs] = {ADD, ADDC, SUB, SUBC,
    DADD, CMP, XOR, AND, OR, BIT, BIC, BIS};
enum opCodes eRegs2[normRegs2] = {MOV, SWAP};
enum opCodes eRegs3[normRegs3] = {SRA, RRC, INVALID, SWPB, SXT};
enum opCodes cpuRegs[cpuInst] = {SETPRI, SVC, SETCC, SETCC, CLRCC, CLRCC};
enum opCodes eCexLdSt[CLS] = {CEX, INVALID, LD, ST};
enum opCodes eMOVx[movInst]= {MOVL, MOVLZ, MOVLS, MOVH};

//table for printing register names and constants
#define r 0
#define c 1
char* RegCons[2][8] = {{"R0", "R1", "R2", "R3", "R4", "R5/LR", "R6/SP", "R7/PC"},
    {"0", "1", "2", "4", "8", "16", "32", "-1"}};

//decode to debug decoding
void decode() {

    #if debug
        //ask user to require the user to hit enter after every instruction of the decode
        printf("\nDecode one instr at a time? (y/n)\n");
        char mode = getchar();
    #endif


    printf("\nDecoding: end address %04x", endAddress);
    char next[inputSiz] = "";
    union reg nextI;
    IR.word = noopInstr;//first instruction to decode is a no-op, before fetching the real first instruction
    //read instructions
    do {
        //decode fetched instruction
        D0();
    #if debug
            if (mode == 'y') {
                //user input to continue
                next[0] = getchar();
            }
    #endif

        //fetch next instruction
        nextI = iFetch(); //F0 and F1

        //decode ends when PC == endAddress +1 because there is not a NO-OP at the start
        //move initial fetch into the loop, and Decode after fetch to fix the issue. The +1 can be removed
    }while(next[0] != 'x' && IR.word != 0 && ProgramRegisters.regConArray[r][pc].word != endAddress);
    printf(("\n")); //extra new line
}

void D0() {
    D0cex();


    if(BubbleBit) {
        BubbleBit = 1; //set BubbleBit since decode must perform an operation
        sprintf(Dmessage, "Bubble\0");
        return;
    }
    #if verbose
        int RC =0,WB=0,SRC=0,DST=0;
    #endif
    toExe = IR; //the full instruction is passed to the executor to access the DST, SRC, etc...
    //read first 3 bits of IR
    unsigned char topCheck;
    topCheck = IR.MOVS.first;
    switch(topCheck) {
        case BLx:
            //BL instruction
    #if verbose
                sprintf(Dmessage,"%s\0", BLch);
    #endif
            nextInstrType = BL;
        break;
        case BXX:
            //Branch Instruction
#if verbose
            sprintf(Dmessage,"%s\0", aBranch[IR.BRANCH.spec]);
#endif
            nextInstrType = eBranches[IR.BRANCH.spec];
        break;
        case REGx:
            //Reg instruction
            int rType = IR.REGS.RegC; //determine subtype of Reg Operation
            if(IR.REGS.RegC < normRegs) {
                //ADD through BIS
                nextInstrType = eRegs1[IR.REGS.RegC];
                #if verbose
                    RC = IR.REGS.RC;
                    WB = IR.REGS.WB;
                    SRC = IR.REGS.SC;
                    DST = IR.REGS.DST;
                    sprintf(Dmessage,"%s RC: %d WB: %d SRC/CON: %s DST: %s\0",
                        aRegs1[IR.REGS.RegC], RC, WB, RegCons[RC][SRC], RegCons[r][DST]);
                #endif
            }
            else if(rType == normRegs) {
                //MOV or SWAP
                nextInstrType = eRegs2[IR.REGS.RC];
                #if verbose
                    WB = IR.REGS.WB;
                    SRC = IR.REGS.SC;
                    DST = IR.REGS.DST;
                    sprintf(Dmessage,"%s WB: %d SRC: %s DST: %s\0",
                        aRegs2[IR.REGS.RC], WB, RegCons[r][SRC], RegCons[r][DST]);
                #endif
            }
            else if(rType == normRegs+1) {
                //SRA through CLRCC
                switch(IR.CPU.sev){
                    case 0:
                        //SRA though SXT
                        nextInstrType = eRegs3[IR.REGS.SC];
#if verbose
                        WB = IR.REGS.WB;
                        SRC = IR.REGS.SC;
                        DST = IR.REGS.DST;
                        sprintf(Dmessage,"%s WB: %d DST: %s\0",
                        aRegs3[IR.REGS.SC], WB, RegCons[r][DST]);
#endif
                    break;
                    case 1:
                        nextInstrType = cpuRegs[IR.CPU.CPUcode];
#if verbose
                        sprintf(Dmessage, "%s\0",cpuStr[IR.CPU.CPUcode] );
#endif
                        break;
                }

            }
            else if(rType > CLScode) {

                nextInstrType =eCexLdSt[IR.CEX.CexLdStCode];
#ifdef verbose
                sprintf(Dmessage, "%s\0", aCexLdSt[IR.CEX.CexLdStCode]);
#endif
            }
            else {
                sprintf(Dmessage,"%04x Unkown CPU or LD-ST Instuction\0", IR.word);
            }
        break;

        case MOVx:
            //MOV instruction
            //See next 2 bits to determine specific MOV instruction
            nextInstrType = eMOVx[IR.MOVS.mc];
            #if verbose
            int mType = IR.MOVS.mc;
            sprintf(Dmessage, "%s Byte: %02x DST: %s\0", aMOVx[IR.MOVS.mc], IR.MOVS.byte, RegCons[r][IR.MOVS.DST]);
            #endif
        break;
        case LDRx:
        case LDRx+1:
            //LDR instruction
            nextInstrType = LDR;
            #if verbose
                sprintf(Dmessage, "-LDR\0");
            #endif
        break;
        case STRx:
        case STRx+1:
            //STR instruction
            nextInstrType = STR;
            #if verbose
                sprintf(Dmessage, "-STR\0");
            #endif
        break;
        default:
            sprintf(Dmessage, "invalid Instruction\0");
        break;
    }
}