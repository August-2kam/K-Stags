#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "machine.h"

#ifdef  DEBUG
    #include <ncurses.h>


#endif

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


//this modifies the instruction pointer ; must be  reset 
// before calling the runProgram  function
void OpcodeIntoCodeArea(machine *m, opcode a)
{
    m->code[m->ip++] = a ;
}


unsigned int
getCode(machine *m)
{
    unsigned int size;
    OpcodeIntoCodeArea(m, PUSH);
    OpcodeIntoCodeArea(m, 1);
    OpcodeIntoCodeArea(m, PUSH);
    OpcodeIntoCodeArea(m, 2);
    OpcodeIntoCodeArea(m, ADD);
    OpcodeIntoCodeArea(m, PRINTI);
    OpcodeIntoCodeArea(m, HALT);

    size = m->ip;

    //reset the ip
    m->ip = 0;

    return size;
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


#ifdef DEBUG
void
runProgram(machine *m)
{
    bool running = true;
    int tos, sos;

//wait for user to input start ...

loop:
        //fetch instruction
        opcode curr = fetch(m);


        //execute
        switch(curr)
        {
            case PUSH:
                int f = fetch(m);
                push(m,f);


                printf("PUSH  %d\n",f);
                break;
            case ADD:
                tos = pop(m);
                sos = pop(m);
                push(m , sos + tos);   //TODO : check for overflow 
                
                printf("ADD\n");
                break;
            case PRINTI:
                tos = pop(m);

                printf("----------------------\n");
                printf("%d\n", tos);
                printf("----------------------\n");



                printf("PRINTI\n");
                break;

            case HALT:
                running = false ;

                printf("HALT\n");
                goto exit;
                break;
            default:
                fprintf(stderr, "Unrecognized opcode\n");
                break;
        }


        char q;
        do
        {

            scanf("%c", &q);
            if(q == 'n') goto loop;
        }
        while(q != 'q');
exit:
    

}



#else
void 
runProgram(machine *m)
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

#endif

