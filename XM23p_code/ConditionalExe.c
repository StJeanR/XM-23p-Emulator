//
// Created by Raymond St-Jean on 2024-07-30.
// Source file for the Conditional Execute instruction code
// the E0 phase of CEX sets up the True and False counts (how many instructions are executed based on Condition)
// D0 and E0 are modified to bubble and decrement the T/F count after a CEX

#include "ConditionalExe.h"
#include <stdbool.h>

#include "RegisterFile.h"

unsigned int TFcase; //1 if True condition, 0 if false consition
unsigned int Tcount; //count of True condition instructions to execute
unsigned int Fcount; //count of False con inst to execute

static inline void checkCon(bool tf) {
            if (tf)
                TFcase = true;
            else
                TFcase = false;
}

void ConExe(unsigned int Cond, unsigned char Tval, unsigned char Fval) {

    Tcount = Tval;
    Fcount = Fval;

    //switch case to check the condition
    switch(Cond) {
        case EQ:
            checkCon(PSW.bits.zero);
        break;

        case NE:
            checkCon(!PSW.bits.zero);
        break;

        case CS:
            checkCon(PSW.bits.carry);
        break;

        case CC:
            checkCon(!PSW.bits.carry);
        break;

        case MI:
            checkCon (PSW.bits.negative);
        break;

        case PL:
            checkCon(!PSW.bits.negative);
        break;

        case VS:
            checkCon(PSW.bits.overflow);
        break;

        case VC:
            checkCon(!PSW.bits.overflow);
        break;

        case HI:
            checkCon( PSW.bits.carry && !PSW.bits.zero);
        break;

        case LS:
            checkCon( !PSW.bits.carry && PSW.bits.zero);
        break;

        case GE:
            checkCon( PSW.bits.negative == PSW.bits.overflow);
        break;

        case LT:
            checkCon( PSW.bits.negative != PSW.bits.overflow);
        break;

        case GT:
            checkCon( !PSW.bits.zero && PSW.bits.negative == PSW.bits.overflow);
        break;

        case LE:
            checkCon( PSW.bits.zero | (PSW.bits.negative != PSW.bits.overflow));
        break;
        
        case TR:
            TFcase = true;
        break;

        case FL:
            TFcase = false;
        break;
    }
}

//CEX check for D0 stages, decides to bubble or not, and decrements the T/F count
void D0cex () {
    if(TFcase == TRUE) { //True CEX case
        if(Tcount > 0) {
            //decode the true instruction
            Tcount--;
        }
        else if (Fcount > 0){
            BubbleBit = set;
            Fcount --;
        }
    }
    else { //False CEX case
        if(Tcount > 0) {
            BubbleBit = set;
            Tcount --;; //the clock cycle after the jump to false instructions
        }
        else if(Fcount > 0) {
            Fcount --;
        }
    }
}

//computer exits from the CEX state (happens on a branch or jump)
void clearCEX() {
    Tcount = 0;
    Fcount = 0;
}
