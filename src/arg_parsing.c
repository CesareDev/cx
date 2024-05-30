#include <stdio.h>

#include "arg_parsing.h"
#include "file_handling.h"

int parse_argument(int arg_count, char* arg_vector[])
{
    if (arg_count == 1)
    {
        printf("Usage: %s [-options] <file-name>\n", arg_vector[0]);
        printf("Options:    '-h' Print this help page\n");
        return 1;
    }

    if (arg_count == 2)
    {
        char* cmd_input = arg_vector[1];

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
                printf("Usage: %s [-options] <file-name>\n", arg_vector[0]);
                printf("Options:    '-h' Help\n");
                return 0;
            }
            else 
            {
                printf("Invalid options, run: %s -h for more information\n", arg_vector[0]);
                return 1;
            }
        }

        return handle_input(cmd_input);
    }

    if (arg_count > 2)
    {
        printf("To many arguments, type '%s -h' for help\n", arg_vector[0]);
        return 1;
    }

    return 0;
}
