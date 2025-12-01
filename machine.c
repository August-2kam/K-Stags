#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "machine.h"

#ifdef  DEBUG
    #include <ncurses.h>
    #include "debugScreen.h"

#endif

int 
initMachine(machine *m)
{
    m->ip = 0;
    m->sp = MAX_AREA - 1;    //MEMORY heap at start-->[   ]<-- stack at the end 
    m->hp = 0;
    
    m->code = calloc(MAX_AREA, sizeof(int));
    m->mem =  calloc(MAX_AREA, sizeof(int));

    
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

void opcodeIntoMemory(machine *m, opcode a)
{
    m->code[m->ip++] = a ;
}
//
void operandIntoMemory(machine *m, int k)
{

    m->code[m->ip++] = k ;

}


unsigned int
getCode(machine *m)
{
    unsigned int size;
    OpcodeIntoCodeArea(m, PUSH);
    OpcodeIntoCodeArea(m, 1);
    OpcodeIntoCodeArea(m, PUSH);
    OpcodeIntoCodeArea(m, 2);
    OpcodeIntoCodeArea(m, PUSH);
    OpcodeIntoCodeArea(m, 2);
    OpcodeIntoCodeArea(m, PUSH);
    OpcodeIntoCodeArea(m, 5);
    OpcodeIntoCodeArea(m, ADD);
    OpcodeIntoCodeArea(m, SUB);
    OpcodeIntoCodeArea(m, MUL);
    OpcodeIntoCodeArea(m, PRINTI);
    OpcodeIntoCodeArea(m, HALT);

    size = m->ip;

    //reset the ip
    m->ip = 0;

    return size;
}



//fetch opcode from the code area 
static inline opcode 
fetch(machine *m )
{
    if(m->ip >= MAX_AREA - 1)
    {
        return NO_OPCODE;

    }

    return m->code[m->ip++];
}


//push operand into the stack
static inline bool  
push(machine *m , int k)
{
    if(m->sp <= m->hp) return false;   // stack-heap colission
    

    m->mem[m->sp--] = k;

    return true ;
}

static inline void
branch(machine *m , uint32_t address)
{
    m->ip = address;

}



//pop operand from the stack
int 
pop(machine *m)
{
    if(m->sp >= MAX_AREA) return 0 ; //nothing to pop
    int k = m->mem[++m->sp];
    m->mem[m->sp] = 0;
    return k;
}


void 
runProgram(machine *m)
{
    bool running = true;
    int tos , sos ;
    uint32_t addr;

#ifdef DEBUG
loop:
#else
    while(running)
    {
#endif
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
                sos = pop(m);
                int sum = sos + tos;
                push(m , sum);   //TODO : check for overflow
                break;

            case SUB:
                tos = pop(m);
                sos = pop(m);
                int sub = sos - tos;
                push(m , sub);   //TODO : check for overflow
                break;

            case MUL:
                tos = pop(m);
                sos = pop(m);
                int mul = sos * tos;
                push(m , mul);   //TODO : check for overflow
                break;

            case DIV:
                tos = pop(m);
                sos = pop(m);
                int div = sos / tos;      //check for error div
                push(m , div);   //TODO : check for overflow
                break;

            case MOD:
                tos = pop(m);
                sos = pop(m);
                int mod = sos % tos;      //check for error div
                push(m , mod);   //TODO : check for overflow
                break;

            case BGT:
                tos = pop(m);
                sos = pop(m);
                addr = fetch(m);

                if(sos > tos) branch(m,addr);
                break;


            case BGE:
                tos = pop(m);
                sos = pop(m);
                addr = fetch(m);

                if(sos >= tos) branch(m,addr);
                break;

            case BLT:
                tos = pop(m);
                sos = pop(m);
                addr = fetch(m);

                if(sos < tos) branch(m,addr);
                break;

            case BLE:
                tos = pop(m);
                sos = pop(m);
                addr = fetch(m);

                if(sos <= tos) branch(m,addr);
                break;
    
            case BEQ:
                tos = pop(m);
                sos = pop(m);
                addr = fetch(m);

                if(sos == tos) branch(m,addr);
                break;

            case BNE:
                tos = pop(m);
                sos = pop(m);
                addr = fetch(m);

                if(sos != tos) branch(m,addr);
                    break;

            case PRINTI:
                tos = pop(m);
                printf("%d\n", tos);
                break;
            case HALT:
                running = false ;
#ifdef DEBUG
                goto exit; 
#endif 
                break;
            default:
                fprintf(stderr, "Unrecognized opcode");
                break;
        }
#ifdef DEBUG
        while(true)
        {
            int ch = getch();
            if(ch == '\n' || ch == KEY_ENTER || ch == '\r') 
            {
               updateScreen(m); 
               goto loop;
            }
        }

exit:
        endwin();
#else 
    }
#endif 


}



