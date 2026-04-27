// Raymond St-Jean
// 2024-05-23.
//ECED 3403 - FileLoad - Loads xme s-records into xm23p emulator program memory

#ifndef FILELOAD_H
#define FILELOAD_H

#define fileNameLen 51  //max size of .asm file name



void Load();

void s0 (int address, signed char* chkSum);
void s1_2 (int len, int address, int mType, signed char* chkSum);
void s9 (int address);
void checkSum(signed char* chkSum);

#endif //FILELOAD_H
