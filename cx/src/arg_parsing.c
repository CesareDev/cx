#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "arg_parsing.h"
#include "file_handling.h"

int print_help(char* bin_name)
{
    printf("Usage: %s [OPTIONS] <file-name-or-string>\n\nOPTIONS:\n", bin_name);
    printf("    -h:             Print this page\n");
    printf("    -q:             Print the dump without encoding\n");
    printf("    -w:             Print the dump without the colors, useful if the you want to pipe the output\n");
    printf("    -o <file-name>: Write the dump in a '.c' like file\n");
    printf("    -n <length>:    Print the dump with custom row length. If not specified the default row length is 16\n");
    return 0;
}

void quiet_option(Options* options)
{
    options->quiet = true;
}

void output_option(Options* options)
{
    options->output = true;
    // Input sanitazier
    options->file_name = optarg;
}

void offset_options(Options* options)
{
    int offset = atoi(optarg);
    bool valid = offset % 4 == 0 && offset <= 64;
    if (!valid)
    {
        printf("%d not valid offset, please insert a multiple of 4 less or equal than 64\n", offset);
        offset = -1;
    }
    options->offset = offset;
}

void white_options(Options* options)
{
    options->white = true;
}

int parse_argument(int arg_count, char* arg_vector[])
{
    int opt;

    Options cmd_opts = {
        false,
        false,
        false,
        16,
        ""
    };
    
    // https://www.gnu.org/software/libc/manual/html_node/Using-Getopt.html
    while ((opt = getopt(arg_count, arg_vector, "hqwo:n:")) != -1)
    {
        switch (opt) {
            case 'h': return print_help(arg_vector[0]);
            case 'q': quiet_option(&cmd_opts); break;
            case 'w': white_options(&cmd_opts); break;
            case 'o': output_option(&cmd_opts); break;
            case 'n': offset_options(&cmd_opts); break;
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
