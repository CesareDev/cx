#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include <stdbool.h>

typedef struct {
    bool quiet;
    bool output;
} Options;

int print_hex(char* input, Options options);

#endif // FILE_HANDLING_H
