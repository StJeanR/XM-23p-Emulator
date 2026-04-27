//
// Created by raymond St-Jean on 2024-06-21.
//source file for executing instructions

#include "instructionExecuter.h"

#include "Decoder_D0.h"

#define rot 1
#define lsb 0
#define LdStOffset 0
#define twoComp 1
#define decBase 10
#define sevMSB 6//7 bit MSB
#define byteMSB 7
#define wordMSB 15
//macros for use in Ld St Pre and Post additions structure
#define pre 1
#define post 0
#define incB 1
#define decB -1
#define incW 2
#define decW -2
#define PRPOcomb 4
#define binary 2
#define dec -1

#define ten 9
#define sign10 0xFC00
#define thirteen 12
#define sign13 0xE000

#define exeErr "Error: instruction not identified in current version"

//the instruction bits to execute
Reg toExe;
//structure for Pre and Post incrementing
Pre_Po PPIncDec;
//table for masking the MSB of a word or a byte
char msbTable[biArray] = {wordMSB,byteMSB};
//Table for setting the high byte in sign extend
char sxtTable[biArray] = {ByteClear, ByteSet};
short sxt10Table[biArray] = {0000, sign10};
short sxt13Table[biArray] = {0, sign13};
//Table for Pre/Post Increment/Decrement. by 1 for Byte, by 2 for word
short IncDecTable[binary][binary][binary] = {{{0, decW}, {incW, 0}},{{0, decB}, {incB, 0}}};

//global variables used for temporary operations
short tempWord;//used for SUB operations to calculate negative value of source
char tempByte; //used to temporarily store byte values

//The Execute stage
void E0() {
    if(BubbleBit) {
        BubbleBit = 0;
        return;
    }
    //switch to find which instruction type was previously decoded
    switch(nextInstrType) {
        case BL:
            tempWord = clr; //clear tempWord
            tempWord = toExe.BL.OFF;
            regWord(r, lr) = regWord(r,pc) - pcIncrease; //store current address into link register
            //add offset to PC
                /*tempWord = tempWord << 1; Do not need to left shift because,
                                            PC uses word address*/
            tempWord = tempWord | sxt13Table[checkBIT(tempWord,thirteen)];
            brWithOffset(tempWord);
            return;
        case BEQ:
            //branch if zero bit set
            if(PSW.bits.zero) {
                branch10Bit(toExe.BRANCH.OFF);
            }
            return;
        case BNE:
            //branch if zero bit not set
            if(PSW.bits.zero == clr) {
               branch10Bit(toExe.BRANCH.OFF);
            }
            return;
        case BC:
            //branch if carry bit set
            if(PSW.bits.carry) {
                branch10Bit(toExe.BRANCH.OFF);
            }
            return;
        case BNC:
            //branch if carry not set
            if(PSW.bits.carry == clr) {
                branch10Bit(toExe.BRANCH.OFF);
            }
            return;
        case BN:
            //branch if N set
            if(PSW.bits.negative) {
                branch10Bit(toExe.BRANCH.OFF);
            }
            return;
        case BGE:
            //branch if overflow not set xor negative bit set
            if(PSW.bits.negative ^ !PSW.bits.overflow) {
                branch10Bit(toExe.BRANCH.OFF);
            }
            return;
        case BLT:
            //branch if overflow xor N set
            if(PSW.bits.negative ^ PSW.bits.overflow) {
                branch10Bit(toExe.BRANCH.OFF);
            }
            return;
        case BRA:
            //always branch
            branch10Bit(toExe.BRANCH.OFF);
            return;
        case ADD:
            //Add two registers, storing the result in Dst
            AddWithCarry(regWord(rRC,rSource), dest, clr);
        break;
        case ADDC:
            //Add two registers plus the carry bit
            AddWithCarry(regWord(rRC,rSource), dest, PSW.bits.carry);
        break;
        case SUB:
            //Subtract one register from the destination
            tempWord = ~(regWord(rRC,rSource)); //negative value of source
            AddWithCarry(tempWord, dest, twoComp);
        break;
        case SUBC:
            //same as SUB, subtracting the carry
            tempWord = ~regWord(rRC,rSource); //negative value of source
            AddWithCarry(tempWord, dest, PSW.bits.carry);
        break;
        case DADD:
            //only changes the carry bit - ignore PSW documentation
            //add each nibble together, with carry from the previous nibble addition
            char car = 0;
            car = nibbleAdd(&tempByte, regNibble(r,dest,n0), regNibble(rRC, rSource, n0));  //add first nibble
            regNibble(r,dest,n0) = tempByte;    //store nibble addition result into register
            car = nibbleAdd(&tempByte, regNibble(r,dest,n1), regNibble(rRC, rSource, n1)+car);  //add subsequent nibbles with carry from previous add
            regNibble(r,dest,n1) = tempByte;
            if(rBW == clr) {//upper byte addition
                car = nibbleAdd(&tempByte, regNibble(r,dest,n2), regNibble(rRC, rSource, n2)+car);
                regNibble(r,dest,n2) = tempByte;
                car = nibbleAdd(&tempByte, regNibble(r,dest,n3), regNibble(rRC, rSource, n3)+car);
                regNibble(r,dest,n3) = tempByte;
            }
            PSW.bits.carry = car;
            clearV();
        break;
        case CMP:
            //set PSW bits depending on result of dst - src
            if(rBW) {
                //byte operation
                tempByte = regByte(r,loByte,dest) - regByte(rRC,loByte, rSource);
                checkZ(tempByte);
                checkCV(sign(tempByte), sign(~(regByte(rRC, loByte,rSource)) + twoComp),
                    sign(regByte(r,loByte,dest)));
            }
            else {
                tempWord = regWord(r,dest) - regWord(rRC,rSource);
                checkZ(tempWord);
                checkCV(sign(tempWord), sign(~(regWord(rRC, rSource)) + twoComp),
                    sign(regWord(r,dest)));
            }
        return; //do not set zero or negative bit based on dest value, bits are already set
        case XOR:
            //XOR two registers, store the result in DST
            if(rBW) {
                //byte
                regByte(r,dest, loByte) = regByte(r,dest,loByte)^regByte(rRC,rSource,loByte);
            }
            else {
                //word
                regWord(r,dest) = regWord(r,dest)^regWord(rRC,rSource);
            }
        break;
        case AND:
            //AND each bit in two registers, store result in DST
            if(rBW) {
                //byte
                regByte(r,dest, loByte) = regByte(r,dest,loByte)&regByte(rRC,rSource,loByte);
            }
            else {
                //word
                regWord(r,dest) = regWord(r,dest)&regWord(rRC,rSource);
            }
        break;
        case OR:
            //OR each bit in two registers, store result in dst
            if(rBW) {
                //byte
                regByte(r,dest, loByte) = regByte(r,dest,loByte)|regByte(rRC,rSource,loByte);
            }
            else {
                //word
                regWord(r,dest) = regWord(r,dest)|regWord(rRC,rSource);
            }
        break;
        case BIT:
            //Set zero bit if given bit is 0, in SRC register
            checkZ(checkBIT(regWord(r,dest), bitOffset(regWord(rRC,rSource))));
            return; //do not check the full register for zero
        break;
        case BIC:
            //clear a bit in DST
            regWord(r,dest) &= ~( set << bitOffset(regWord(rRC,rSource)) );
        break;
        case BIS:
            //set a bit in DST
            regWord(r,dest) |= ( set << bitOffset(regWord(rRC,rSource)) );
        break;
        case MOV:
            regWord(r, dest) = regWord(rBW, rSource);
        break;
        case SWAP:
            //swap the values of two registers
            tempWord = regWord(r,rSource);
            regWord(r,rSource) = regWord(r,dest);
            regWord(r,dest) = tempWord;
        break;
        case SRA:
            //Shift register right, maintaining the MSB
            tempByte = checkBIT(regWord(r,dest),msbTable[rBW]);
            PSW.bits.carry = shiftRight();
            regWord(r,dest) = regWord(r,dest) | tempByte << msbTable[rBW];
        break;
        case RRC:
            //shift register right, making MSB the carry bit
            tempByte = shiftRight(); //save carry result of current operation
            regWord(r,dest) = regWord(r,dest) | PSW.bits.carry << msbTable[rBW]; //use carry from previous operation
            PSW.bits.carry = tempByte; //set carry to current operation carry result
        break;
        case SWPB:
            //swap the bytes in a register
            tempByte = regByte(r, dest, loByte); //save low byte
            regByte(r,dest,loByte) = regByte(r,dest,hiByte); //replace low byte with high byte
            regByte(r,dest,hiByte) = tempByte; //replace high byte with saved low byte
        break;
        case SXT:
            //convert register from byte length to word length
            regByte(r,dest,hiByte) = sxtTable[checkBIT(regByte(r,dest,loByte),byteMSB)];
        break;

        case SETPRI:
            //SETPRI not handled yet
        break;
        case SVC:
            //SVC not handled yet
        break;
        case SETCC:
            //Set the indicated PSW bits, leave other bits unchanged
            OrPSW(ClrSt);
        return;
        case CLRCC:
            //clear the indicated PSW bits, leave other bits unchanged
            OrPSW(ClrSt);
            togglePSW(ClrSt);
        return;

        case CEX:
            //Pass the C F and T from the instruction to a function that handles CEX setup
            ConExe(toExe.CEX.C, toExe.CEX.T, toExe.CEX.F);
        return; //do not change PSW
        case LD:
            //Load From DataMemory into an indicated Register
            LoadStore(rSource);
            DCTRL.reg = dest;   //for E1, register to load to
            DCTRL.LS = l;       //signal E1 to load
        break; //do not change PSW
        case ST:
            //Store from register into DMEM
            LoadStore(dest);
            DCTRL.reg = rSource;    //set register for E1 to store from
            DCTRL.LS = s;           //signal E1 to store
        break; //do not change PSW

        case MOVL:
            //set lowbyte to given byte value in instruction
            setByte(dest, loByte, mByte);
        break;
        case MOVLZ:
            //Mov value to low byte of register, setting high byte to 0
            setByte(dest, hiByte, sxtTable[clr]); //set high byte to 0x00
            setByte(dest, loByte, mByte);   //set lowbyte to given value
        break;
        case MOVLS:
            //move value onto low byte of dst, setting the high byte to 0xFF
            setByte(dest, hiByte, sxtTable[set]); //set high byte to 0xFF
            setByte(dest, loByte, mByte); //set lowbyte to given value
        break;
        case MOVH:
            //sets the high byte to given value, leaving the lowbyte alone
            setByte(dest, hiByte, mByte);
        break;

        case LDR:
            sendDMAR(regWord(r,rSource), toExe.LDRSTR.rOffset);  //send Memory address to E1
            DCTRL.reg = dest;    //set register for E1 to load to
            DCTRL.LS = l;           //signal E1 to load
        return; //do not change PSW
        case STR:
            sendDMAR(regWord(r,dest), toExe.LDRSTR.rOffset);  //send Memory address to E1
            DCTRL.reg = rSource;    //set register for E1 to store from
            DCTRL.LS = s;           //signal E1 to store
        return;  //do not change PSW
        default:
            //error, instruction incorrectly identified or is not handled in this version
            #if debug
                printf(exeErr);
            #endif
        return;

    }

    //check if the result is 0
    checkZ(regWord(r,dest));
    //check MSB for negative value
    checkN(regWord(r,dest));

    if(dest == pc) {
        //set bubble bit if PC was changed
        BubbleBit = set;
        //cancel CEX execution
        clearCEX();
    }
}

//takes a 10 bit offset as input, left shifts it, sign extend the OFFset
void branch10Bit(short OFF){
    //OFF = OFF << 1; do not need to shift as PC uses word address
    OFF = OFF | sxt10Table[checkBIT(OFF,ten)];
    brWithOffset(OFF);
}

//change the pc to branch to a new location
void brWithOffset(short OFF) {
    //change PC to new position
    regWord(r, pc) = regWord(r,pc) + OFF + dec;
    //Set bubble bit to disable D0,E0 for the next cycle
    BubbleBit = set;
    //disable CEX operation
    clearCEX();
}

//Add two registers and store the result in destination, used for ADD, ADDC, SUB, SUBC
void AddWithCarry(int srcVal, int dst, int carry) {
    short dstSign = sign(regWord(r,dst));
    //check if byte operation
    if (rBW == clr) {
        //word operation
        regWord(r,dst) = regWord(r,dst) + srcVal + carry;
    }
    else {
        //byte operation
        char srcValByte = srcVal;//cut high byte from source
        regByte(r,dst,loByte) = regByte(r,dst,loByte) + srcValByte + carry;
    }
    //PSW carry and overflow bits
    checkCV(dstSign, sign(srcVal), sign(regWord(r,dst)));
}

//adds nibbles and returns the carry result, used for DADD
char nibbleAdd(char* result, unsigned char Ndst, unsigned char Nsrc) {
    char tmp = Ndst + Nsrc;
    char carry = clr;
    if(tmp >= decBase) {
        carry = set;
        tmp = tmp - decBase;
    }

    *result = tmp;
    return carry;
}

//shift register right one bit, and return the value of the LSB
char shiftRight() {
    //set carry if LSB is 1
    char tempC = checkBIT(regWord(r,dest),lsb);
    if(rBW) {
        //byte operation
        regByte(r,dest,loByte) = regByte(r,dest,loByte) >> rot;
    }
    else {
        //word operation
        regWord(r, dest) = regWord(r,dest) >> rot;
    }
    return tempC;
}

//returns the byte or word value of the source depending on B/W of the instruction
short bitOffset() {
    short offset;
    if(rBW) {
        //byte source
        offset = regByte(rRC,rSource, loByte);
    }
    else {
        //word source
        offset = regWord(rRC,rSource);
    }
    return offset;
}

//Function for LD and ST with Pre and Post inc/dec
void LoadStore(unsigned int accessReg) {
    PPIncDec.postpre[(PrPo+set)%binary] = clr; //clear the unused part of the pre/post structure
    PPIncDec.postpre[PrPo] = IncDecTable[rBW][LSinc][LSdec]; //set up Pre and Post Increment or Decrement
    regWord(r,accessReg) += PPIncDec.postpre[pre]; //pre inc/dec/none
    sendDMAR(regWord(r,accessReg), LdStOffset);  //send Memory address to E1
    regWord(r,accessReg) += PPIncDec.postpre[post]; //post inc/dec/none
}

//send address to DMAR for E1, for LD/ST and LDR and STR
void sendDMAR(short address, char offset) {
    //perform 7 bit sign extension on the offset
    Reg tempWord;
    tempWord.bytes[loByte] = offset;
    tempWord.bytes[loByte] |= (checkBIT(offset,sevMSB) << byteMSB);
    tempWord.bytes[hiByte] = sxtTable[checkBIT(offset,sevMSB)];

    DMAR = address + tempWord.word;//set DMAR to required address
    //set up DCTRL for E1
    DCTRL.WB = toExe.LDST.WB;   //signal to E1 if word or byte
    DCTRL.memAccess = set; //set memAccess to true
}

//Sets the PSW zero bit if a value is 0
void checkZ(short val) {
    if(val == clr) {
        //if result is 0, set the zero bit
        setZ();
    }
    else {
        //if result is not 0, clear zero bit
        clearZ();
    }
}

//sets the carry and overflow bits based on addition of two values using their MSB
void checkCV(int dstSign, int srcSign, int resultSign) {
    //PSW carry and overflow bits
    PSW.bits.carry = carryTable[srcSign][dstSign][resultSign];
    PSW.bits.overflow = overflowTable[srcSign][dstSign][resultSign];
}

//sets the Negative PSW bit if the passed value is negative
void checkN(short dst) {
    //set N to the MSB of the result
    setN( checkBIT(dst, msbTable[rBW]) );
}

