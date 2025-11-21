#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#include "machine.h"


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
    runProgram(&stags);


    //free machine 
    if(stags.code) free(stags.code);
    if(stags.mem)  free(stags.mem);
    return EXIT_SUCCESS;
}
