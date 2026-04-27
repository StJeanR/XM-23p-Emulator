//
// Created by Raymond St-Jean on 2024-07-06.
//Source file for E1 stage, finishes any instructions that require a second clock cycle

#include "StageE1.h"

//E0 passes value to load or store to DMAR
short DMAR;
short DMBR;
char DMBRbyte;
dmemCTRL DCTRL; //details for E1 execution


//second execute stage. Performs memory transfer for Loading and Storing intructions
void E1() {
    if(DCTRL.memAccess != 1) {
        //only perform E1 if signaled, ie. memAccess is 1
        return;
    }

    switch(DCTRL.WB) {
        case w:
            //word case
            if(DMAR%2 != 0) {
                //word access on an odd address, print warning
                printf("\nWarning: Word Memory access on odd address");
            }

            if(DCTRL.LS == l) { //Load word
                //set DMBR to value in DMEM
                DMBR = PM.MEM[DataIndex].words[DMAR>>1];
                //set register word to data word
                regWord(r,DCTRL.reg) = DMBR;
            }
            else{ //Store word
                //set DMBR to value in register
                DMBR = regWord(r,DCTRL.reg);
                //set value in memory to DMBR
                PM.MEM[DataIndex].words[DMAR>>1] = DMBR;
            }
        break;
        case b:
            //byte case
            if(DCTRL.LS == l) { //Load byte
                //set DMBR to value in DMEM
                DMBRbyte = PM.MEM[DataIndex].bytes[DMAR];
                //set register word to data word
                regByte(r,DCTRL.reg, loByte) = DMBRbyte;
            }
            else{ //Store byte
                //set DMBR to value in register
                DMBRbyte = regByte(r,DCTRL.reg, loByte);
                //set value in memory to DMBR
                PM.MEM[DataIndex].bytes[DMAR] = DMBRbyte;
            }
        break;
    }
#ifdef debug
    sprintf(E1message,"E1:%04x\0", toExe.word);
    DCTRL.memAccess = 0; //set memAccess to false
#endif
}