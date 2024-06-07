#include <stdio.h>
#include <unistd.h>

#include "arg_parsing.h"
#include "file_handling.h"

int print_help(char* bin_name)
{
    printf("Usage %s [OPTIONS] <file-name>\n\nOPTIONS:\n", bin_name);
    printf("-h: Print this page\n");
    printf("-q: Print the dump without encoding\n");
    printf("-o: Dump the output in a file\n");
    return 0;
}

int parse_argument(int arg_count, char* arg_vector[])
{
    int opt;

    Options cmd_opts = {
        false,
        false
    };

    while ((opt = getopt(arg_count, arg_vector, "hqo")) != -1)
    {
        switch (opt) {
            case 'h': return print_help(arg_vector[0]);
            case 'q': cmd_opts.quiet = true; break;
            case 'o': cmd_opts.output = true; break;
            default: printf("Type: %s -h for the help page", arg_vector[0]); return 1;
        }
    }

    if (optind + 1 == arg_count)
    {
        return print_hex(arg_vector[optind], cmd_opts);
    }
    else 
    {
        printf("Too many or not enough argument\nType: %s -h for the help page\n", arg_vector[0]); 
        return 1;
    }

    return 1;
}
