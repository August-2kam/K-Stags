#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define currChar(b) b->buffer[b->bufPointer]
#define advanceBufPointer(b) b->bufPointer++
#define endOfBuffer(b) (b)->bufPointer >= (b)->bufferSize
#define INT_STRING  "0123456789"



typedef struct 
{
    char    *buffer;
    size_t  bufferSize;
    size_t  bufPointer;
    int col,line;
}srcBuffer;

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

    printf("ghere\n");

    char c = currChar(sb);
    while(c != '\n' ||
          c != ' '  ||
          c != '\0') 
    {
        printf("%c\n", c);
        size++;
        advanceBufPointer(sb);
        c = currChar(sb);
    }


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
        advanceBufPointer(sb);
    }

    
    return neg ? -1 * val : val;

}


void 
printMnemonic(srcBuffer *sb)
{
    skipSpaces(sb);
    char *name;
    int num;

    if(isalpha(currChar(sb)))
    {
        printf("here\n");
        int size = readMnemonic(sb, &name);
        printf("here\n");
        char *pname = malloc(size + 1);
        strncpy(pname , name , size);
        printf("%s\n", pname);
        free(pname);

    }
    else if(isdigit(currChar(sb)) || currChar(sb)=='-')
    {
        int num = readInt(sb);
        printf("%d\n", num);

    }

}


void
assemble(char *filename ,int *mem)
{
    //initalize the source buffer struct
    srcBuffer sb = (srcBuffer){0};
    sb.buffer = NULL;
    sb.bufferSize = utility_fileReader(filename, &sb.buffer);
    sb.bufPointer = 0;
    sb.line = 0;

    printf("%s\n", sb.buffer);


    skipSpaces(&sb);
    printMnemonic(&sb);

    /*
    while(sb.bufferSize > sb.bufPointer)
    {


    }*/

}


int main(int argc , char *argv[] )
{
    int mem[1000];
    assemble(argv[1], mem);

    return 0;


}

