//
// Created by Raymond St-Jean on 2024-05-29.
//Decoder header for lab 2

#ifndef DECODER_D0_H
#define DECODER_D0_H

#include "Memory_Loader.h"
#include "Fetcher.h"
#include "FileLoad.h"
#include "instructionExecuter.h"
#include "ConditionalExe.h"

enum Maddr {BLx, BXX, REGx, MOVx, LDRx, STRx = 6};

extern short endAddress;
enum opCodes{INVALID = -1, BL,BEQ, BNE, BC, BNC, BN, BGE, BLT, BRA,
            ADD, ADDC, SUB, SUBC, DADD, CMP, XOR, AND, OR, BIT, BIC, BIS, MOV, SWAP,
            SRA, RRC, SWPB, SXT, SETPRI, SVC, SETCC, CLRCC, CEX, LD, ST,
            MOVL, MOVLZ, MOVLS, MOVH, LDR, STR};
extern enum opCodes nextInstrType;

union instruction {
    unsigned short word;
    unsigned char bytes[wordBytes];

};

void decode();
void D0();

#endif //DECODER_D0_H
