#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define MAX_AREA 1024


typedef struct 
{
 //registers 
 uint32_t ip;       //current instruction in the code area we are executing
 uint32_t sp;       // pointer to the top of the stack in memory
 uint32_t hp;       // pointer to the top of the heap  in memory


 //code 
 int* code;

 //memory
 int* mem;
 
} machine;


int 
initMachine(machine *m)
{
    m->ip = 0;
    m->sp = MAX_AREA - 1;    //MEMORY heap at start-->[   ]<-- stack at the end 
    m->hp = 0;
    
    m->code = malloc(MAX_AREA);
    m->mem =  malloc(MAX_AREA);

    
    if(!m->code) return  EXIT_FAILURE; 
    if(!m->mem) return   EXIT_FAILURE; 

    return EXIT_SUCCESS;

}


int 
main(int argc, char *argv[])
{
    machine stags = (machine){0};
    if(initMachine(&stags) == EXIT_FAILURE)
    { 
        fprintf(stderr, "Failed to init the Stags Machine");

        return EXIT_FAILURE;
    }

    //run 

    //free machine 
    if(stags.code) free(stags.code);
    if(stags.mem)  free(stags.mem);
    return EXIT_SUCCESS;
}
