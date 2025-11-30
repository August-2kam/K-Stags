#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_STRING  "0123456789"

typedef struct {
    char    *buffer;
    size_t  bufferSize;
    size_t  bufPointer;
    int     col, line;
} srcBuffer;

/* safer file reader - returns 0 on failure, file size on success */
size_t utility_fileReader(const char *filename, char **input_buffer) {
    FILE *f = fopen(filename, "rb");
    size_t file_size = 0;

    if (!f) return 0;   // failed to open the file

    fseek(f, 0, SEEK_END);
    file_size = (size_t)ftell(f);
    rewind(f);

    *input_buffer = malloc(file_size + 1);
    if (!*input_buffer) {
        fclose(f);
        return 0;
    }

    fread(*input_buffer, 1, file_size, f);
    (*input_buffer)[file_size] = '\0';  // sentinel
    fclose(f);

    return file_size;
}

/* safe peek: returns '\0' if at or past end of buffer */
static char peekChar(srcBuffer *b) {
    if (!b->buffer || b->bufPointer >= b->bufferSize) return '\0';
    return b->buffer[b->bufPointer];
}

/* advance pointer but never pass bufferSize */
static void advance(srcBuffer *b) {
    if (b->bufPointer < b->bufferSize) b->bufPointer++;
}

/* skip whitespace and update line count */
void skipSpaces(srcBuffer *sb) {
    char c = peekChar(sb);
    while (c != '\0' && isspace((unsigned char)c)) {
        if (c == '\n') sb->line++;
        advance(sb);
        c = peekChar(sb);
    }
}

/* return index of character in DIGITS or -1 */
static int charToInt(const char *dig, char c) {
    char *p = strchr(dig, c);
    return p ? (int)(p - dig) : -1;
}

/* read integer (handles optional leading '-') */
static int readInt(srcBuffer *sb) {
    int k, val = 0, neg = 0;
    char c = peekChar(sb);

    if (c == '-') {
        neg = 1;
        advance(sb);
        c = peekChar(sb);
    }

    if (!isdigit((unsigned char)c)) {
        fprintf(stderr, "ASVM: line %d: expected a digit after optional sign\n", sb->line);
        exit(1);
    }

    k = charToInt(INT_STRING, c);
    while (k >= 0) {
        val = val * 10 + k;
        advance(sb);
        c = peekChar(sb);
        k = charToInt(INT_STRING, c);
    }

    return neg ? -val : val;
}

/* read mnemonic: returns length and sets *start to pointer inside buffer */
int readMnemonic(srcBuffer *sb, char **start) {
    skipSpaces(sb);
    *start = sb->buffer + sb->bufPointer;
    int size = 0;
    char c = peekChar(sb);

    /* stop at whitespace, comma, or NUL or end-of-buffer */
    while (c != '\0' && !isspace((unsigned char)c) && c != ',' ) {
        size++;
        advance(sb);
        c = peekChar(sb);
    }

    return size;
}

void printMnemonic(srcBuffer *sb) {
    skipSpaces(sb);
    char *name;
    if (isalpha((unsigned char)peekChar(sb))) {
        int size = readMnemonic(sb, &name);
        if (size <= 0) return;
        char *pname = malloc(size + 1);
        if (!pname) {
            fprintf(stderr, "Out of memory\n");
            exit(1);
        }
        strncpy(pname, name, size);
        pname[size] = '\0'; /* important: terminate */
        printf("MNEMONIC: '%s'\n", pname);
        free(pname);
    } else if (isdigit((unsigned char)peekChar(sb)) || peekChar(sb) == '-') {
        int num = readInt(sb);
        printf("NUMBER: %d\n", num);
    } else if (peekChar(sb) == '\0') {
        /* nothing to do */
    } else {
        /* unknown char - skip single char to avoid infinite loop */
        printf("UNKNOWN CHAR '%c' at line %d\n", peekChar(sb), sb->line);
        advance(sb);
    }
}

void assemble(char *filename, int *mem) {
    if (!filename) {
        fprintf(stderr, "Usage: assemble <sourcefile>\n");
        exit(1);
    }

    srcBuffer sb = {0};
    sb.buffer = NULL;
    sb.bufferSize = utility_fileReader(filename, &sb.buffer);
    sb.bufPointer = 0;
    sb.line = 1; /* 1-based line count */

    if (sb.bufferSize == 0 || sb.buffer == NULL) {
        fprintf(stderr, "Failed to read file '%s'\n", filename);
        exit(1);
    }

    /* For demonstration, print whole buffer then parse first token */
    printf("Source:\n%s\n----\n", sb.buffer);


    while (sb.bufPointer < sb.bufferSize) {
        skipSpaces(&sb);
        if (peekChar(&sb) == '\0') break;
        printMnemonic(&sb);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <sourcefile>\n", argv[0]);
        return 1;
    }
    int mem[1000] = {0};
    assemble(argv[1], mem);
    return 0;
}
