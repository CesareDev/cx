#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "cxapi.h"

HexBuffer get_hex(const char* filename)
{
    HexBuffer output = { NULL, 0 };
    // Check if the input is a directory
    struct stat path_stat;
    int stat_result = stat(filename, &path_stat);
    if (stat_result != 0 || S_ISDIR(path_stat.st_mode))
    {
        return output;
    }

    FILE* file_buffer = fopen(filename, "rb"); 
    if (file_buffer == NULL)
    {
        return output;
    }

    // Get the file size in byte
    // Move the pointer at the end of the file
    if (fseek(file_buffer, 0L, SEEK_END) != 0) 
    {
        fclose(file_buffer);
        return output;
    }

    // Calculate the size in byte of the file
    long file_size = ftell(file_buffer);

    if (file_size < 0)
    {
        fclose(file_buffer);
        return output;
    }
    
    // Put the pointer at the start of the file
    rewind(file_buffer);

    if (file_size == 0)
    {
        fclose(file_buffer);
        return output;
    }

    unsigned char* buffer = (unsigned  char*)malloc(file_size);

    int current_byte = getc(file_buffer);
    unsigned long offset = 0;

    while(current_byte != EOF || offset < file_size)
    {
        unsigned char encoded_byte = (unsigned char)current_byte;
        // Add the byte to the output buffer
        if (current_byte != EOF)
        {
            buffer[offset] = encoded_byte;
            offset++;
        }

        current_byte = getc(file_buffer);
    }

    output.buffer = buffer;
    output.size = file_size;

    return output;
}

void free_hex(HexBuffer buffer)
{
    if (buffer.buffer != NULL)
    {
        free(buffer.buffer);
    }
}
