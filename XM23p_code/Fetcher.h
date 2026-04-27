//
// Created by Raymond St-Jean on 2024-05-29.
//Fetch header for Lab2

#ifndef FETCHER_H
#define FETCHER_H

#define pcIncrease 1
#define bit 1
#define wordBytes 2
#define CLSsize 2
#define movOP 2
#define topCode 3
#define regCode 3
#define RtypeCode 5
#define pswSize 5
//register codes
#define lr 5    //R5 - Link Register
#define sp 6    //R6 - Stack Pointer
#define pc 7    //R7 - Program Counter
#define relOffset 7
#define nibSize 4
#define byteSz 8
#define BLoffset 13
#define Bxoffset 10

extern union reg IR;
extern union reg IMBR;
extern union reg IMAR;
extern int clk;

typedef struct bl {
    unsigned int OFF : BLoffset;
    unsigned int first:topCode;
}bl;

typedef struct brx {
    unsigned int OFF : Bxoffset;
    unsigned int spec :regCode;
    unsigned int first : topCode;
}brx;

typedef struct movs {
    //struct allocates low bits first
    unsigned int DST : regCode;
    //MOVx Instruction type
    unsigned int byte : byteSz;
    unsigned int mc : movOP; //2 bits specify which MOVx
    unsigned int first : topCode; //3 most significant bits filters code into 6 groups
}movs;

//topCode refers to an instruction from ADD to SXT
typedef struct regOps {
    //struct allocates low bits first
    unsigned int DST : regCode;
    //REGx Instruction type
    unsigned int SC : regCode;
    unsigned int WB : bit;
    unsigned int RC : bit;
    unsigned int RegC : RtypeCode;
    unsigned int first : topCode; //3 most significant bits filters code into 6 groups
}regOps;

typedef struct SetClr {
    unsigned int pswCC : pswSize;
    unsigned int buf : regCode; //bits 7,6,5, not used by setClr
    //same high byte as regOps
    unsigned int RegC : RtypeCode; //5 bits specify which subgroup of topcode 010
    unsigned int first : topCode;
}SetClr;

//used to distinguish between SRA-SXT, and for instructions SETPRI and SVC
typedef struct CPUtype {
    unsigned int PR_SA :nibSize;
    unsigned int CPUcode : regCode;
    unsigned int sev : bit; //bit 7 distinguishes between SRA-SXT and SVC-CLRCC
    //same high byte as regOps
    unsigned int RegC : RtypeCode; //5 bits specify which subgroup of topcode 010
    unsigned int first : topCode;
}CPUtype;

typedef struct CEX {
    unsigned int F : regCode;
    unsigned int T : regCode;
    unsigned int C : nibSize;
    unsigned int CexLdStCode : CLSsize;
    unsigned int topNib : nibSize;
}ConditionalEX;

typedef struct LdSt {
    unsigned int DST : regCode;
    unsigned int SRC : regCode;
    unsigned int WB : bit;
    unsigned int INC : bit;
    unsigned int DEC : bit;
    unsigned int PRPO : bit;
    unsigned int topSix : 6; //top six bits of the instruction
}LdSt;

typedef struct LdrStr {
    unsigned int DST : regCode;
    unsigned int SRC : regCode;
    unsigned int WB : bit;
    unsigned int rOffset : relOffset;
    unsigned int top2 :  2; //top 2 bits of the instruction
} LdrStr;

typedef struct Nibbles {
    unsigned int n0 : nibSize;
    unsigned int n1 : nibSize;
    unsigned int n2 : nibSize;
    unsigned int n3 : nibSize;
}Nibbles;

//union to access program memory using byte and/or word addresses
typedef union reg {
    //word size
    unsigned short word;
    //byte accessing
    unsigned char bytes[wordBytes];
    //nibble accessing (for DADD)
    Nibbles nibbles;
    //bit accesing for decode
    bl BL;//Need structure for Branches
    brx BRANCH;
    movs MOVS;  //structure for Mov instructions
    regOps REGS;    //structure for REG instructions
    SetClr PSW;     //Structure for STCC and CLRCC
    CPUtype CPU;    //structure for the lower register/Cpu instructions
    ConditionalEX CEX;  //Structure for Conditional execute, and LD ST
    LdSt LDST;          //Structure for Load and Store instructions
    LdrStr LDRSTR;
    //need structure for LDR and STR
}Reg;


void initFetch();
union reg iFetch();
union reg F0();
union reg F1();

#endif //FETCHER_H
