#include <stdio.h>
#include <signal.h>

//#include "FileLoad.h"
//#include "Memory_Loader.h"
//#include "Fetcher.h"
#include "Decoder_D0.h"
#include "Clock.h"
#include <corecrt.h>

#define empty 0

int main(void)
{
    //initialise SIGING for ^C control
    ctrl_c_fnd = FALSE;
#ifdef _MSC_VER
    signal(SIGINT, (_crt_signal_t)sigint_hdlr); //reinitialize SIGINT
#elif __GNUC__
    signal(SIGINT, (__p_sig_fn_t)sigint_hdlr);
#endif

    //load file
    Load();
    //initialise constants in the register file
    initRegs();
    //allow user to view memory and debug
    memAccess();

    //end of program
    getchar();
    return 0;
}
