// Raymond St-Jean
// 2024-05-23.
//ECED 3403 - FileLoad - Loads xme s-records into xm23p emulator program memory

#include "FileLoad.h"

#include "Fetcher.h"
#include "Memory_Loader.h"


#define reduceLen 3
#define addressLength 2
#define decSize 10
#define validChk -1

FILE* Lfile;

void Load() {
    char loadFile[fileNameLen];
    int instructionLength = clr;
    //reset clock counter to 0 and Initialise IR to a noop
    resetClk();
    //load executable file
    printf("provide name of file to load: ");
    fscanf(stdin, " %50[^\n]0", loadFile);
    getchar();
    Lfile = fopen(loadFile,"r");

    if(Lfile == NULL) {
        printf("File failed to open.\n");
        return;
    }

    //Read executable line by line
    char type, nextChar;
    int address;
    int len;
    signed char chkSum; //Variable used for checksum

    do {
        //clear variables
        address = clr;
        chkSum = clr;
        //read 'S'
        nextChar = fgetc(Lfile);
        if (nextChar != 'S')
        {
            printf("error, wrong format: (Record Missing 'S')\n");
            return;
        }
        //read the record type
        type = fgetc(Lfile);
        //read the length
        nextChar = fgetc(Lfile);
        len = HexToDec(nextChar);   //first hex value is the most significant
        len = len << mul16;               //so multiply by 16 first
        len += HexToDec((fgetc(Lfile)));
        chkSum += len;
        len = len - reduceLen;  //All records have a 2-byte address, and a 1-byte chksum. len is now length of data

        //Read address in two bytes
        short adrByte;
        for(int i = addressLength;i > 0; i--) {
            address = (address << mul16) << mul16;     //multiply by 16 twice for next byte
            adrByte = HexToDec(fgetc(Lfile));
            adrByte = adrByte << mul16;
            adrByte += HexToDec(fgetc(Lfile));
            chkSum += adrByte;
            address += adrByte;
        }



        //for each type
        if (type == '0') {
            s0(len, &chkSum);
        }
        else if (type =='1') {
            //instructions
            s1_2(len, address, InstIndex, &chkSum);
            instructionLength += len;
        }
        else if (type == '2') {
            //data
            s1_2(len,address, DataIndex, &chkSum);
        }
        else if (type == '9')
            s9(address);

        //perform checksum
        checkSum(&chkSum);
        //eat newline character
        fgetc(Lfile);


    }while(type != '9');
    //Done reading after reading an S9 record

   initFetch();
    printf(".xme successfuly Loaded\nSource File asm: %s\nStart Address: %04x\n", PM.asmFileName, PM.startAddress);
    endAddress = ((PM.startAddress+instructionLength)>>set)+2;
}


//store name of .asm file
void s0 (int len, signed char* chkSum) {
    char fileChar;
    //read len characters for the name of the file
    int i;
    for (i = 0; i<len;i++) {
        fileChar =HexToDec(fgetc(Lfile));
        fileChar = fileChar << mul16;
        fileChar += HexToDec(fgetc(Lfile));
        PM.asmFileName[i] = fileChar;
        *chkSum += fileChar;
    }
    PM.asmFileName[len] = 0;
    //the name of the file is now stored in Program Memory
}

//load record into IMEM or DMEM
void s1_2 (int len, int address, int mType, signed char* chkSum) {
    char tempByte;

    for (int i = 0; i<len; i++) {
        //Byte to be loaded to IMEM or DMEM
        tempByte = HexToDec(fgetc(Lfile));
        tempByte = tempByte << mul16;
        tempByte += HexToDec((fgetc(Lfile)));
        PM.MEM[mType].bytes[address+i] = tempByte;
        *chkSum += tempByte;
    }
}

//set starting address of program
void s9 (int address) {
    PM.startAddress = address;
}

//perform the checksum to ensure that the record is not corrupted
void checkSum(signed char* chkSum) {
    char check = HexToDec(fgetc(Lfile));
    check = check << mul16;
    check += HexToDec(fgetc(Lfile));
    *chkSum += check;
    if (*chkSum != validChk)
        printf("Check: %d \n", *chkSum);
}