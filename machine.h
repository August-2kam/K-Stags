#pragma  once 

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

int initMachine(machine *m);
unsigned int getCode(machine *m);
void runProgram(machine *m);
