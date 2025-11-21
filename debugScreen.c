
#include <ncurses.h>
#include "debugScreen.h"
#include "machine.h"



unsigned int codeSize;

unsigned int SCREEN_WID;
unsigned int SCREEN_HEI;            
unsigned int currentLine;

void 
setDebugScreens(unsigned int size)
{
     codeSize = size;
     getmaxyx(stdscr, SCREEN_HEI, SCREEN_WID);      //find the boundaries of the screen
     currentLine = 0;
}


const char* opcodeToString(const opcode op)
{
    switch(op)
    {
        case PUSH:      return "PUSH";
        case ADD:       return "ADD";
        case PRINTI:    return "PRINTI";
        case HALT:      return "HALT";
        default: return "NOP";

    }


}

void
setScreen(machine *m)
{

    initscr();


    //print the code
    int k = 0;
    opcode op ;
    while(k <= codeSize + 1)
    {
        op = m->mem[k];
        switch(op)
        {
            // <OPCODE>   <OPERAND>
            case PUSH:
                printw("%s   %d\n",opcodeToString(op), m->mem[k++] );
                break;

            // <OPCODE>
            case ADD:
            case HALT:
                printw("%s\n", opcodeToString(op) );
                break;
                
            default:
                printw("UNRECOGNIZED OP\n");
                break;
        }

        refresh();
    }

    
    return;

}

void

updateScreen(machine *m)
{
    return;

}

