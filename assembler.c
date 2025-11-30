#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define INT_STRING  "0123456789"

size_t utility_fileReader(const char *filename , char **input_buffer)
{
    FILE *f = fopen(filename, "rb");
    unsigned int file_size = 0;

    if (!f) return -1;   //failed to opne the file

    //fetch the size of the file
    fseek(f, 0, SEEK_END);
    file_size = ftell(f);
    rewind(f);

    //read file contents into the buffer
    *input_buffer= malloc(file_size + 1);
    fread(*input_buffer, 1, file_size, f);
    (*input_buffer)[file_size] = '\0';  // Sentinel
    fclose(f);

    return file_size;

}

typedef struct 
{
    char    *buffer;
    size_t  bufferSize;
    size_t  bufPointer;
    int col,line;
}srcBuffer;

#define currChar(b) b->buffer[bufPointer]
#define advanceBufPointer(b) b->bufPointer++
#define endOfBuffer(b) b->bufPointer >= b->bufferSize

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


//isolate the mnemonic in the buffer 
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

//read an int 
static int 
charToInt(const char *dig , char c)
{
    char *p;
    p = strchr(dig , c);
    return p ? p -dig : -1 ;
}

static int 
readInt(srcBuffer *sb)
{
    int k , val = 0 , neg = 0;
    if(currChar(sb) == '-')
    {
        neg = 1;
        advanceBufPointer(sb);
    }

    if(!isdigit(currChar(sb)))
    {
        fprintf(stderr, "ASVM: line %ud:Expected a digit after the negative sign\n" , sb->line);
        exit(1);
    }

    k = charToInt(INT_STRING ,currChar(sb));

    while(k >= 0)
    {
        val = val*10 + k ;
        k = charToInt(INT_STRING ,currChar(sb));
    }

    
    return neg ? -1 * val : val;

}







void
assemble(char *filename ,int **mem)
{
    srcBuffer sb = (srcBuffer){0};
    utility_fileReader(filename, &sb->buffer);




}






