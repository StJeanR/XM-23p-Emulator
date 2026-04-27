//
// Created by Raymond St-Jean on 2024-06-23.
// funtions for modifying the PSW bits

#include "PSW.h"

#include "RegisterFile.h"

unsigned int overflowTable[biArray][biArray][biArray] = {0,1,0,0,0,0,1,0};
unsigned int carryTable[biArray][biArray][biArray] = {0,0,1,0,1,0,1,1};

union statusReg PSW;

void setC() {
    PSW.bits.carry = set;
}
void clearC() {
    PSW.bits.carry = clr;
}
void setN(int newN) {
    PSW.bits.negative = newN;
}
void clearN() {
    PSW.bits.negative = clr;
}
void setZ() {
    PSW.bits.zero = set;
}
void clearZ() {
    PSW.bits.zero = clr;
}
void setV() {
    PSW.bits.overflow = set;
}
void clearV() {
    PSW.bits.overflow = clr;
}


void OrPSW(int toSet) {
    PSW.comb.psw = PSW.comb.psw | toSet;
}
void togglePSW(int toTog) {
    PSW.comb.psw = PSW.comb.psw ^ toTog;
}
void displayPSW() {
    printf(" V:%d SLP:%d N:%d Z:%d C:%d",
        PSW.bits.overflow, PSW.bits.sleep, PSW.bits.negative,PSW.bits.zero, PSW.bits.carry);
}