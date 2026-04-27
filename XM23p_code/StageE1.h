//
// Created by Raymond St-Jean on 2024-07-06.
//header for E1 stage, finishes any instructions that require a second clock cycle

#ifndef STAGEE1_H
#define STAGEE1_H

#include "instructionExecuter.h"
#include "Clock.h"


//Load or Store bit value
#define l 0
#define s 1

extern short DMAR; //value passed by E0 to load or store

//structure for DCTRL for data Memory access
typedef struct dmemCTRL {
    unsigned int memAccess : bit; //1 if Ld/St has been performed
    unsigned int WB : bit;          //Word or byte access
    unsigned int LS : bit;          //signals if load(0) or store(0)
    unsigned int reg : regCode;     //the code of the register to store from or load to
}dmemCTRL;

extern dmemCTRL DCTRL;

void E1();

void load();
void store();

#endif //STAGEE1_H
