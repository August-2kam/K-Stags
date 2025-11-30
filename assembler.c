#include <stdio.h>


#define currChar(b) b->buffer[bufPointer]
#define advanceBufPointer(b) b->bufPointer++
#define endOfBuffer(b) b->bufPointer >= b->bufferSize

typedef struct 
{
    char    *buffer;
    size_t  bufferSize;
    size_t  bufPointer;
    int col,line;
}srcBuffer;



//skip white spaces in the 
void 
skipSpaces(srcBuffer *sb)
{
    while(!(endOfBuffer(sb)) && isspace(currChar(sb)))
    {
        if(currChar(sb) == '\n') sb->line++;
        advanceBufPointer(sb);

    }
}

int
readMnemonic(srcBuffer *sb, char **start)
{
    skipSpaces(sb);
    *start = sb->buffer + sb->bufPointer;
    int size = 0;

    char c = currChar(sb);
    while(c != '\n' ||
          c != ' '  ||
          c != '\0') size++;


    return size ;    
}





void
assemble(char *filename ,int **mem)
{


}






