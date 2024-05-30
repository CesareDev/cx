#include <ctype.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>

#define OFFSET_LIMIT 16

// Thanks to https://stackoverflow.com/a/27125283
// Usage: printf("%s This line is red\n", KRED);
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

double custom_log(double base, double x) 
{
    return log(x) / log(base);
}

void print_encoding_char(int not_encoded_byte)
{
    // Not printable character
    if (not_encoded_byte < 32 || !isascii(not_encoded_byte))
    {
        // Add a "0" to the byte < 16 for better formatting
        if (not_encoded_byte < 16)
        {
            printf("%s0%hhx%s ", KYEL, not_encoded_byte, KNRM);
        }
        else 
        {
            printf("%s%hhx%s ", KYEL, not_encoded_byte, KNRM);
        } 
    }
    else 
    {
        printf("%s%hhx%s ", KGRN, not_encoded_byte, KNRM);
    }
}

int file_handling(const char* input)
{
    // Check if the input is a directory
    struct stat path_stat;
    stat(input, &path_stat);
    if (S_ISDIR(path_stat.st_mode))
    {
        printf("Can't open a directory\n");
        return 1;
    }

    // Open the file
    FILE* file_buffer = fopen(input, "rb");  

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

    // Start reading the file and format the output
    
    // Read the first byte
    int current_byte = getc(file_buffer);
    unsigned int byte_offset = 1;
    unsigned long offset = 0;

    // Calculate how many digits are needed for all the line in the file
    // 10 -> Decimal representation
    // 16 -> Hexadecimal representation
    // ...
    unsigned int offset_digits = (int)custom_log(10, file_size) + 1;
    printf("%0*lx: ", offset_digits, offset);

    unsigned char line_encoding[16] = { 0 };

    while (current_byte != EOF)
    {
        unsigned char encoded_byte = (unsigned char)current_byte;

        // Non printable charcter -> control character or non ASCII character
        if (encoded_byte < 32 || !isascii(current_byte))
        {
            encoded_byte = '.';
        }
        line_encoding[byte_offset - 1] = encoded_byte;

        print_encoding_char(current_byte);

        current_byte = getc(file_buffer);

        // After printing 8 byte add a "separator"
        if (byte_offset == 8)
        {
            printf("  ");
        }
        byte_offset++; 
        
        // After print all the 16 (OFFSET_LIMIT) byte go to new line and print the offset 
        if (byte_offset > OFFSET_LIMIT && current_byte != EOF)
        {
            printf("| %s", line_encoding);
            printf("\n");
            byte_offset = 1;
            offset += OFFSET_LIMIT;
            if (offset < file_size)
            {
                printf("%0*ld: ", offset_digits, offset);
            }
            // Clear the buffer
            for (int i = 0; i < OFFSET_LIMIT; i++)
            {
                line_encoding[i] = 0;
            }
        } 

        // Print the encoded line even if we don't exeed the byte offset limit
        // The last line could be shortet
        if (current_byte == EOF)
        {
            int offset_diff = OFFSET_LIMIT - (byte_offset - 1);
            int printing_offset = offset_diff * 3;
            if (byte_offset <= 8) 
            {
                // Space between the 8 bytes groups
                printing_offset += 2; 
            }
            printf("%*s| %s\n", printing_offset, "", line_encoding);
        }
    }

    // Close the file buffer
    fclose(file_buffer);

    return 0;
}

int main(int argc, char* argv[]) 
{
    if (argc <= 1)
    {
        printf("Usage: cx [-options] <file-name>\n");
        printf("Options:    '-h' Print this help page\n");
        return 1;
    }

    if (argc == 2)
    {
        char* cmd_input = argv[1];

        char cmd_end_line = '\0';
        size_t cmd_input_size = 0;
        while (cmd_input[cmd_input_size] != cmd_end_line)
        {
            cmd_input_size++;
        }

        // Parse options
        if (cmd_input_size == 2 && cmd_input[0] == '-')
        {
            if (cmd_input[1] == 'h')
            {
                printf("Usage: cx <file-name>\n\n");
                printf("Options:    '-h' Help\n");
                return 0;
            }
            else 
            {
                printf("Invalid options, run: cx -h for more information\n");
                return 1;
            }
        }

        return file_handling(cmd_input);
    }

    if (argc > 2)
    {
        printf("To many arguments, type 'cx -h' for help\n");
        return 1;
    }

    return 0;
}
