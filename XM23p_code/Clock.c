//
// Created by Raymond St-Jean on 2024-06-25.
// Clock cycle and 'go' functions for running programs with the emulator

#include "Clock.h"
#include "Memory_Loader.h"

char Dmessage[DmessageSize];
char E1message[DmessageSize];
char BubbleBit;
char mode;
int clockCount;
char cpu_go;
volatile sig_atomic_t ctrl_c_fnd; //indicates ^C detected

void go() {
    cpu_go = TRUE;
    ctrl_c_fnd = FALSE;

    #if debug
    //ask user to require the user to hit enter after every clock cycle
    printf("\nPerform one clock cycle at a time? (y/n)\n");
    mode = getchar();
    printf("Clock\tPC\tInstruction\tFetch\tDecode\tExecute\n");
    #endif

    if(clockCount == clr) {
        IR.word = noopInstr;//first instruction to decode is a no-op, before fetching the real first instruction
    }

    //run until breakpoint and odd clock
    while( (regWord(r,pc) != endAddress || clockCount%2 == 0) && cpu_go) {
        clock();

        #ifdef debug
                if(mode == 'y') {//wait for user to continue
                    if (getchar() == 'r') {
                        debugger();
                    }
                }
        #endif
        cpu_go = !ctrl_c_fnd;
    }

    printf("\n--------------------------------\nRun Terminated - Breakpoint reached\n");
    printf(optionMessage);
}

void clock() {

    if (clockCount%2 == 0) {
        //even clock, do F0, D0
        IMAR = F0();
        D0();
        E1();
    }
    else {
        //odd clock, do F1, E0
        IMBR = F1();
        IR = IMBR;
        E0();
    }
#if debug
    //print clock count
    printCycle();
#endif
    clockCount++;
}

//reset the clock and other variable to run a new program
void resetClk() {
    clockCount = 0;
    IR.word = noopInstr;
}

void printCycle() {
    printf("\n%d\t", clockCount);
    if(clockCount%2 == 0) {
        //even clock, print F0, D0
        printf("%04x\t%04x\t\tF0:%04x D0:%04x\t",(ProgramRegisters.regConArray[r][pc].word-pcIncrease)<<1,
            PM.MEM[InstIndex].words[IMAR.word], IMAR.word<<1, toExe.word);
        if(E1message[0] != 0) {
            //print E1 message if there is one
            printf(E1message);
            E1message[0] = 0;
        }
#if verbose
        printf("\t\t");   //Verbose mode prints the decoded instruction
        printf(Dmessage);
#endif
    }
    else {
        //odd clock, print F1, E0
        printf("\t\t\tF1:%04x\t\tE0:%04x\t", IMBR.word, toExe.word);
#if verbose
        printf("\tResult: %04x \t", regWord(r,dest)); //Too verbose
        displayPSW();
#endif
    }
}

void sigint_hdlr() {
    //invoked when ^C is detected
    ctrl_c_fnd = TRUE;
#ifdef _MSC_VER
    signal(SIGINT, (_crt_signal_t)sigint_hdlr); //reinitialize SIGINT
#elif __GNUC__    //__p_sig_fn_t
    signal(SIGINT, (__p_sig_fn_t)sigint_hdlr);
#endif
}