#include <stdio.h>
#include <cxapi.h>

int main(int argc, char* argv[]) 
{
    // Create a hello.txt file in the same directory of the executable
    HexData data = get_hex("hello.txt");

    for (long i = 0; i < data.size; i++)
    {
        printf("Hex: %hhx\n", data.buffer[i]);
    }

    free_hex(data);

    return 0;
}
