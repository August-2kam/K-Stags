#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#include "machine.h"

#ifdef DEBUG 
    #include "debugScreen.h"
#endif

int 
main(int argc, char *argv[])
{   
 
    machine stags = (machine){0};
    if(initMachine(&stags) == EXIT_FAILURE)
    { 
        fprintf(stderr, "Failed to init the Stags Machine");

        return EXIT_FAILURE;
    }

    //getCode 
    unsigned int size = getCode(&stags);

    //run 
#ifdef DEBUG
    setScreen(&stags);
#endif
    runProgram(&stags);


    //free machine 
    if(stags.code) free(stags.code);
    if(stags.mem)  free(stags.mem);
    return EXIT_SUCCESS;
}
