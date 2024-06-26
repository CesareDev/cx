#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>

#include "file_handling.h"
#include "print_macro.h"

#define MAX_INPUT_SIZE 1024

int print_hex(char* input, Options options)
{
    // Information of the error in arg_parsing.c file
    if (options.offset < 0)
    {
        return 1;
    }

    // Check if the input is a directory
    struct stat path_stat;
    int stat_result = stat(input, &path_stat); 

    // Not a file
    bool input_is_file = true;
    if (stat_result != 0)
    {
        input_is_file = false;
    }

    if (S_ISDIR(path_stat.st_mode))
    {
        printf("Can't open a directory\n");
        return 1;
    } 

    // Open the file or write into the stream if the input is a string

    FILE* file_buffer = NULL;
    if (input_is_file)
    {
        file_buffer = fopen(input, "rb"); 
    }
    else 
    {
        unsigned long input_size = strnlen(input, MAX_INPUT_SIZE);
        file_buffer = fmemopen(input, input_size + 1, "rb");
    }

    if (file_buffer == NULL)
    {
        printf("File doesn't exist\n");
        return 1;
    }

    // Get the file size in byte
    // Move the pointer at the end of the file
    if (fseek(file_buffer, 0L, SEEK_END) != 0) 
    {
        printf("Error while reading the file\n");
        fclose(file_buffer);
        return 1;
    }

    // Calculate the size in byte of the file
    long file_size = ftell(file_buffer);

    if (file_size < 0)
    {
        printf("File to big");
        fclose(file_buffer);
        return 1;
    }
    // Put the pointer at the start of the file
    rewind(file_buffer);

    if (file_size == 0)
    {
        printf("File empty\n");
        fclose(file_buffer);
        return 0;
    }

    // End of the checks

    // Start reading the file and format the output
    
    unsigned char output_buffer[file_size];
    long output_index = 0;

    // Read the first byte
    const int OFFSET_LIMIT = options.offset; 
    int current_byte = getc(file_buffer);
    unsigned int byte_offset = 1;
    unsigned long offset = 0;

    // Calculate how many digits are needed for all the line in the file
    // 10 -> Decimal representation
    // 16 -> Hexadecimal representation
    // ...
    //
    unsigned int offset_digits = (int)(log(file_size) / log(10)) + 1;
    if (!options.quiet)
    {
        printf("Structure:\n<byte-offset> : <content> | <encoding>\n\n");
        printf("- %0*lx: ", offset_digits, offset);
    }

    typedef struct 
    {
        unsigned char character;
        bool is_ascii;
        bool printable;
    } encode;

    encode line_encoding[OFFSET_LIMIT];

    while (current_byte != EOF)
    {
        unsigned char encoded_byte = (unsigned char)current_byte;
        bool ascii = isascii(current_byte);
        bool printable = true;

        // Add the byte to the output buffer
        if (options.output && current_byte != EOF)
        {
            output_buffer[output_index] = encoded_byte;
            output_index++;
        }

        // Non printable charcter -> control character or non ASCII character 
        if (!ascii)
        {
            encoded_byte = '?';
            printable = false;
            // Add a "0" to the byte < 16 for better formatting
            if (current_byte < 16)
            {
                if (options.white)
                {
                    printf("0%hhx ", current_byte); 
                }
                else 
                {
                    printf("%s0%hhx%s ", RED, current_byte, NRM); 
                }
            } 
            else 
            {
                if (options.white)
                {
                    printf("%hhx ", current_byte); 
                }
                else 
                {
                    printf("%s%hhx%s ", RED, current_byte, NRM);
                }
            }
        }
        else if (current_byte < 32)
        {
            encoded_byte = '.';
            printable = false;
            // Add a "0" to the byte < 16 for better formatting
            if (current_byte < 16)
            { 
                if (options.white)
                {
                    printf("0%hhx ", current_byte);
                }
                else 
                {
                    printf("%s0%hhx%s ", YEL, current_byte, NRM);
                }
            } 
            else 
            {
                if (options.white)
                {
                    printf("%hhx ", current_byte); 
                }
                else 
                {
                    printf("%s%hhx%s ", YEL, current_byte, NRM);
                }
            } 
        }
        else 
        {
            if (options.white)
            {
                printf("%hhx ", current_byte); 
            }
            else 
            {
                printf("%s%hhx%s ", GRN, current_byte, NRM);
            }
        }

        line_encoding[byte_offset - 1].character = encoded_byte;
        line_encoding[byte_offset - 1].is_ascii = ascii;
        line_encoding[byte_offset - 1].printable = printable;

        current_byte = getc(file_buffer);

        // After printing 8 byte add a "separator"
        if (byte_offset == OFFSET_LIMIT / 2 && !options.quiet)
        {
            printf("  ");
        }
        byte_offset++; 

        // After print all the 16 (OFFSET_LIMIT) byte go to new line and print the offset  
        if (byte_offset > OFFSET_LIMIT && current_byte != EOF)
        {
            if (options.quiet)
            {
                printf("\n");
                byte_offset = 1;
                offset += OFFSET_LIMIT; 
                continue;
            }
            printf("| ");
            for (unsigned char i = 0; i < OFFSET_LIMIT; i++)
            {
                if (options.white)
                {
                    printf("%c", line_encoding[i].character);
                }
                else if (line_encoding[i].is_ascii && line_encoding[i].printable)
                {
                    printf("%c", line_encoding[i].character);
                }
                else if (!line_encoding[i].printable && line_encoding[i].is_ascii)
                {
                    printf("%s%c%s", YEL, line_encoding[i].character, NRM);
                }
                else 
                {
                    printf("%s%c%s", RED, line_encoding[i].character, NRM);
                } 
            }
            printf("\n");
            byte_offset = 1;
            offset += OFFSET_LIMIT;
            if (offset < file_size)
            {
                printf("- %0*ld: ", offset_digits, offset);
            }
            // Clear the buffer
            for (int i = 0; i < OFFSET_LIMIT; i++)
            {
                line_encoding[i].character = 0;
                line_encoding[i].is_ascii = true;
                line_encoding[i].printable = true;
            }
        } 

        // Print the encoded line even if we don't exeed the byte offset limit
        // The last line could be shortet
        if (current_byte == EOF)
        {
            int offset_diff = OFFSET_LIMIT - (byte_offset - 1);
            for (int i = 0; i < offset_diff; i++)
            {
                // Add a space if we surpass the first half of byte
                int space_threshol = offset_diff - (OFFSET_LIMIT / 2);
                if (space_threshol > 0 && i == space_threshol && !options.quiet)
                {
                    printf("  ");
                }

                // Placeholder character
                if (!options.quiet)
                {
                    if (options.white)
                    {
                        printf("-- ");
                    }
                    else
                    {
                        printf("%s--%s ", BLU, NRM);
                    }
                } 
            }
            if (options.quiet)
            {
                printf("\n");
                continue;
            }
            printf("| ");
            for (unsigned char i = 0; i < OFFSET_LIMIT; i++)
            {
                // Stop printing if we arrive at the last available character
                if (line_encoding[i].character == 0)
                {
                    break;
                }
                if (options.white)
                {
                    printf("%c", line_encoding[i].character);
                }
                else if (line_encoding[i].is_ascii && line_encoding[i].printable)
                {
                    printf("%c", line_encoding[i].character);
                }
                else if (!line_encoding[i].printable && line_encoding[i].is_ascii)
                {
                    printf("%s%c%s", YEL, line_encoding[i].character, NRM);
                }
                else 
                {
                    printf("%s%c%s", RED, line_encoding[i].character, NRM);
                }
            }
            printf("\n");
        }
    }

    if (!options.quiet)
    {
        if (options.white)
        {
            printf("\nTotal file size: %ld byte\n", file_size);
        }
        else 
        {
            printf("\nTotal file size: %s%s%ld byte%s\n", BOLD, RED, file_size, NRM);
        }
    }

    // Close the file buffer
    fclose(file_buffer);

    // Output a file
    if (!options.output)
    {
        return 0;
    }

    FILE* output_file;
    output_file = fopen(options.file_name, "w");
    if (output_file == NULL)
    {
        printf("Error creating the file");
        return 1;
    }

    fprintf(output_file, "unsigned char hex_dump[%ld] = { ", file_size);

    for (long i = 0; i < file_size; i++)
    {
        if (i < file_size - 1)
        {
            fprintf(output_file, "0x%hhx, ", output_buffer[i]);
        }
        else 
        {
            fprintf(output_file, "0x%hhx", output_buffer[i]);
        }
    }

    fprintf(output_file, " };\n");

    fclose(output_file);

    return 0;
}
