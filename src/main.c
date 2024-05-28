#include <stdio.h>

int main(int argc, char* argv[]) 
{
    if (argc <= 1)
    {
        printf("You need to select a file to dump!\n");
        return 1;
    }

    if (argc > 2)
    {
        printf("To many arguments, type 'cx -h' for help\n");
        return 1;
    }

    // Here the argument lenght is 2

    char* input = argv[1];

    char end_line = '\0';
    size_t input_size = 0;
    while (input[input_size] != end_line)
    {
        input_size++;
    }

    // Parse options
    if (input_size == 2 && input[0] == '-')
    {
        if (input[1] == 'h')
        {
            printf("Usage: cx <file-name>\n\n");
            printf("Options:    '-h' Help\n");
        }
    }

    return 0;
}
