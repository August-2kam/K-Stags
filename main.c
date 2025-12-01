#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#include "machine.h"

unsigned int assemble(char *filename , machine *m);

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
    unsigned int size = assemble(argv[1], &stags);
    printf("%d\n", size);

    //run 
#ifdef DEBUG
    setDebugScreenProps(size);
    setScreen(&stags);
#endif
    runProgram(&stags);


    //free machine 
    if(stags.code) free(stags.code);
    if(stags.mem)  free(stags.mem);

    //destroy the windows
#ifdef DEBUG
    destroyWindows();
#endif
    return EXIT_SUCCESS;
}
