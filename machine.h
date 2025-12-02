#pragma  once 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


#define MAX_AREA 1024


typedef enum
{
    ALLOC,
    PUSH,
    ADD,
    MUL,
    SUB,
    DIV,
    MOD,
    LDA,
    VAL,
    STO,
    PRINTI,
    BEQ,
    BNE,
    BLT,
    BLE,
    BGT,
    BGE,
    NOT,
    JUMP,
    JUMPTO,
    HALT,

    NO_OPCODE
}opcode;

#define  NUM_REGISTERS 3
typedef struct 
{
    //registers 
    uint32_t ip;       //current instruction in the code area we are executing
    uint32_t sp;       // pointer to the top of the stack in memory
    uint32_t hp;       // pointer to the top of the heap  in memory
    uint32_t fp;       //frame pointer  for the vaiables 


    //code 
    int* code;

    //memory
     int* mem;
 
} machine;

int initMachine(machine *m);
unsigned int getCode(machine *m);
void runProgram(machine *m);
void opcodeIntoMemory(machine *,opcode m);
void operandIntoMemory(machine *m,int k);
const char *opcodeToMnemonic(opcode op);
