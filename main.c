#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_AREA 1024


typedef enum
{
    PUSH,
    ADD, 
    PRINTI,
    HALT,

    NO_OPCODE
}opcode;

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

void OpcodeIntoCodeArea(machine *m, opcode a)
{
    m->code[m->ip++] = a ;
}

/*
 * Push  1   ;[1]
 * push  2   ;[2,1]
 * ADD       ;[3]
 * PRINTI    ;[] ->PRINT 3 HERE 
 * HALT 
 *
*/
void
getCode(machine *m)
{
    OpcodeIntoCodeArea(m, PUSH);
    OpcodeIntoCodeArea(m, 1);
    OpcodeIntoCodeArea(m, PUSH);
    OpcodeIntoCodeArea(m, 2);
    OpcodeIntoCodeArea(m, ADD);
    OpcodeIntoCodeArea(m, PRINTI);
    OpcodeIntoCodeArea(m, HALT);


    //reset the ip
    m->ip = 0;
}



//fetch opcode from the code area 
opcode 
fetch(machine *m )
{
    if(m->ip >= MAX_AREA - 1)
    {
        return NO_OPCODE;

    }

    return m->code[m->ip++];
}


//push operand into the stack
bool  
push(machine *m , int k)
{
    if(m->sp <= m->hp) return false;   // stack-heap colission
    

    m->mem[m->sp--] = k;

    return true ;
}



//pop operand from the stack
int 
pop(machine *m)
{
    if(m->sp >= MAX_AREA) return 0 ; //nothing to pop
    
    return m->mem[++m->sp];
}
//[PUSH , 2 , PUSH , 1 ,  ADD , PRINTI , HALT]

void runProgram(machine *m)
{
    bool running = true;
    int tos , sos ;

    while(running)
    {
        //fetch instruction
        opcode curr = fetch(m);


        //execute
        switch(curr)
        {
            case PUSH:
                push(m,fetch(m) );
                break;
            case ADD:
                tos = pop(m);

                printf("%d\n", tos);
                sos = pop(m);

                printf("%d\n", sos);

                push(m , sos + tos);   //TODO : check for overflow 

                break;
            case PRINTI:
                tos = pop(m);

                printf("%d\n", tos);
                break;

            case HALT:
                running = false ;
                break;
            default:
                fprintf(stderr, "Unrecognized opcode");
                break;
        }

    }



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

    //getCode 
    getCode(&stags);

    //run 
    runProgram(&stags);


    //free machine 
    if(stags.code) free(stags.code);
    if(stags.mem)  free(stags.mem);
    return EXIT_SUCCESS;
}
