#include <endian.h>
#include <stdbool.h>
#include <ncurses.h>
#include <stdio.h>
#include "debugScreen.h"
#include "machine.h"



unsigned int codeSize;

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

unsigned int CODE_WIN_X;
unsigned int CODE_WIN_Y;

unsigned int REG_WIN_X;
unsigned int REG_WIN_Y;

unsigned int renLineNum  = 0;
unsigned int currentLine = 0;

bool printColor;

WINDOW *codeWindow;
WINDOW *registersWindow;


WINDOW *create_newwin(int height, int width, int starty, int startx);
void drawWindows();
void destroy_win(WINDOW *local_win);




void 
setDebugScreenProps(unsigned int size)
{
    initscr();
    cbreak();
    nocbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    //window measurements
    SCREEN_WID = COLS;
    SCREEN_HEI = LINES;

    PAD_LEFT   = (int)(SCREEN_WID * PAD_LEFT_PER);
    PAD_RIGHT  = (int)(SCREEN_WID * PAD_RIGHT_PER);
    PAD_TOP    = (int)(SCREEN_HEI * PAD_TOP_PER);
    PAD_BOTTOM = (int)(SCREEN_HEI * PAD_BOTTOM_PER);

    USABLE_W   = SCREEN_WID - PAD_LEFT - PAD_RIGHT;
    USABLE_H   = SCREEN_HEI - PAD_TOP - PAD_BOTTOM;

    WIN_WIDTH  = USABLE_W/2;
    WIN_HEIGHT = USABLE_H;

    CODE_WIN_X = PAD_LEFT;
    CODE_WIN_Y = PAD_TOP;

    REG_WIN_X  = PAD_LEFT + WIN_WIDTH;
    REG_WIN_Y  = PAD_TOP;


    //code rendering properties 
     codeSize = size;
     currentLine = 0;


     //init  the color properties 
    if(has_colors() == FALSE)
	{	
        printColor = false;
        
	}

    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, COLOR_BLUE);


    //create the windows
    codeWindow      = newwin(WIN_HEIGHT, WIN_WIDTH, CODE_WIN_Y, CODE_WIN_X); 
    registersWindow = newwin(WIN_HEIGHT, WIN_WIDTH, REG_WIN_Y, REG_WIN_X);

    //draw the window boxes 
    drawWindows();

    //show initial state 
//    wnoutrefresh(codeWindow);
  

    //outrefresh(registersWindow);
    doupdate();

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
printRegisters(machine *m)
{
    int x = 2;

    mvwprintw(registersWindow, 10 , x , "IP: %d", m->ip  );
    mvwprintw(registersWindow, 15 , x , "SP: %d", m->sp  );
    mvwprintw(registersWindow, 20 , x , "HP: %d", m->hp  );


}

void
printMem(machine *m)
{
    const int X_END = SCREEN_WID - PAD_RIGHT;
    const int X0    = PAD_LEFT;



    //----print the stack----- 
    int y = SCREEN_HEI - PAD_BOTTOM + 5 ;
    int x = PAD_LEFT;


    int k = m->sp;
    while(x < X_END && k < MAX_AREA)
    {
        mvprintw(y, x, "%d ", m->mem[k++]);
        x+= 2;
    }

    //--------print the heap--------- 
    int y1 = SCREEN_HEI - PAD_BOTTOM + 7 ;
    int x1 = PAD_LEFT;


    int k1 = m->hp;
    while(x1 < X_END && k1 >=0)
    {
        mvprintw(y1, x1, "%d ", m->mem[k1--]);
        x1 += 2;
    }

    refresh();
}

void
setScreen(machine *m)
{
    char *currLinePointeri = " ";


    renLineNum = 0;



    //dr
    drawWindows();


    //print the code
    int k = 0;
    opcode op ;
    while(k < codeSize)     
    {
        //set the color if the current  executed line is the one being drawn
        if(currentLine == renLineNum)
        {
            wattron(codeWindow, COLOR_PAIR(1) | A_BOLD);
            currLinePointeri = ">";

        }

        //cordinates inside the inner area
        int y = renLineNum + 1;
        int x = 2;

        //dont print outside the window 
        if(y >= (WIN_WIDTH -1 )) break;


        //get the opcode
        op = m->code[k];

        switch(op)
        {
            // <OPCODE>   <OPERAND>
            case PUSH:
                k+=1;
                mvwprintw(codeWindow, y, x , "%s%s %d", currLinePointeri, opcodeToString(op), m->code[k++]);
                break;

            // <OPCODE>
            case ADD:
            case HALT:
            case PRINTI:
                mvwprintw(codeWindow, y, x, "%s%s",currLinePointeri, opcodeToString(op));
                k++;
                break;
                
            default:
                printw("UNRECOGNIZED OP\n");
                k++;
                break;
        }
        if(currentLine == renLineNum)
        {

            wattroff(codeWindow, COLOR_PAIR(1) | A_BOLD);
            currLinePointeri = " ";
        }

        renLineNum++;
    }

    //print the registers
    printRegisters(m);
    refresh();

    //print the stack an heap
    printMem(m);

    //write to the windows
    refresh();
    wrefresh(codeWindow);
    wrefresh(registersWindow);
    return;

}

void
updateScreen(machine *m)
{
    currentLine++;
    setScreen(m);attroff(COLOR_PAIR(1));

    return;

}




void
drawWindows()
{
    //erase the content 
    werase(codeWindow);
    werase(registersWindow);

    //draw the boxes 
    box(codeWindow, 0 , 0 );
    box(registersWindow, 0, 0);

    //refresh windows
    wrefresh(codeWindow);
    wrefresh(registersWindow);

}

void 
destroyWindows()
{
    if(codeWindow) delwin(codeWindow);
    if(registersWindow) delwin(registersWindow);
    endwin();
}

