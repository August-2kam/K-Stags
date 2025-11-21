#include <complex.h>
#include <ncurses.h>
#include "debugScreen.h"
#include "machine.h"



unsigned int codeSize;
unsigned int currentLine;

unsigned int SCREEN_HEI;
unsigned int SCREEN_WID;


float PAD_LEFT_PER = 0.10;
float PAD_RIGHT_PER = 0.10;
float PAD_TOP_PER = 0.10;
float PAD_BOTTOM_PER = 0.30;

int PAD_LEFT;
int PAD_RIGHT;
int PAD_TOP;
int PAD_BOTTOM;

int USABLE_W;
int USABLE_H;



unsigned int WIN_HEIGHT;
unsigned int WIN_WIDTH;

WINDOW *codeWindow;
WINDOW *registersWindow;


WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);




void 
setDebugScreenProps(unsigned int size)
{
    initscr();

    SCREEN_WID = COLS;
    SCREEN_HEI = LINES;



    PAD_LEFT   = (int)(SCREEN_WID * PAD_LEFT_PER);
    PAD_RIGHT  = (int)(SCREEN_WID * PAD_RIGHT_PER);
    PAD_TOP    = (int)(SCREEN_HEI * PAD_TOP_PER);
    PAD_BOTTOM = (int)(SCREEN_HEI * PAD_BOTTOM_PER);

    USABLE_W   = SCREEN_WID - PAD_LEFT - PAD_RIGHT;
    USABLE_H   = SCREEN_HEI - PAD_TOP - PAD_BOTTOM;

    //code rendering properties 
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
	cbreak();
	keypad(stdscr, TRUE);	

    refresh();
    codeWindow = create_newwin( USABLE_H , USABLE_W/2, PAD_TOP, PAD_LEFT); 
    registersWindow = create_newwin( USABLE_H , USABLE_W/2,PAD_TOP, PAD_LEFT + USABLE_W/2); 

/*
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

*/
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

