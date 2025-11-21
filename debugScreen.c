#include <ncurses.h>
#include "debugScreen.h"
#include "machine.h"



unsigned int codeSize;

unsigned int SCREEN_WID;
unsigned int SCREEN_HEI;            
unsigned int currentLine;

unsigned int PADDING_LEFT;
unsigned int PADDING_RIGHT;
unsigned int PADDING_TOP;
unsigned int PADDING_BOTTOM;

unsigned int WIN_PADDING_BOTTOM;
unsigned int WIN_PADDING_TOP;


unsigned int WIN_HEIGHT;
unsigned int WIN_WIDTH;

WINDOW *codeWindow;
WINDOW *registersWindow;


WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);




void 
setDebugScreenProps(unsigned int size)
{
    //code rendering properties 
     codeSize = size;
     getmaxyx(stdscr, SCREEN_HEI, SCREEN_WID);      //find the boundaries of the screen
     currentLine = 0;

    // windows properties
    PADDING_LEFT   = SCREEN_WID*0.05;
    PADDING_RIGHT  = SCREEN_WID*0.05;

    WIN_PADDING_BOTTOM = SCREEN_HEI*0.7;
    WIN_PADDING_TOP = SCREEN_HEI*0.1;

    WIN_HEIGHT = WIN_PADDING_TOP - WIN_PADDING_BOTTOM;
    WIN_WIDTH  =  SCREEN_WID * 0.6;


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
	cbreak();
	keypad(stdscr, TRUE);	

    codeWindow = create_newwin(50 , 50 , 10 , 10);


    //print the code
    int k = 0;
    opcode op ;
    while(k < codeSize)
    {
        op = m->code[k];
        switch(op)
        {
            // <OPCODE>   <OPERAND>
            case PUSH:
                k+=1;
                printw("%s   %d\n",opcodeToString(op), m->code[k++] );
                break;

            // <OPCODE>
            case ADD:
            case HALT:
            case PRINTI:
                printw("%s\n", opcodeToString(op) );
                k++;
                break;
                
            default:
                printw("UNRECOGNIZED OP\n");
                k++;
                break;
        }

    }
    refresh();
    return;

}

void
updateScreen(machine *m)
{
    return;

}


WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* machine *mShow machine *mthat machine *mbox 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}

