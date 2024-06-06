#include <stdio.h>
#include <unistd.h>

#include "arg_parsing.h"
#include "file_handling.h"

int print_help(char* bin_name)
{
    printf("Usage %s [options] <file-name>\nOptions:\n", bin_name);
    printf("-h: Print this page\n");
    printf("-q: Print the dump without encoding\n");
    printf("-o: Dump the output in a file\n");
    return 0;
}

int print_quiet()
{
    return 0;
}

int write_to_file()
{
    return 0;
}

int parse_argument(int arg_count, char* arg_vector[])
{
    int opt;

    while ((opt = getopt(arg_count, arg_vector, "hqo")) != -1)
    {
        switch (opt) {
            case 'h': return print_help(arg_vector[0]);
            case 'q': return print_quiet();
            case 'o': return write_to_file();
            default: printf("Usage: %s [options] <file-name>\n", arg_vector[0]); return 1;
        }
    }

    if (optind + 1 == arg_count)
    {
        return handle_input(arg_vector[optind]);
    }
    else 
    {
        printf("Too many argument, try %s -h\n", arg_vector[0]);
        return 1;
    }

    return 1;
}
