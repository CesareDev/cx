#include <stdio.h>
#include <cxapi.h>

int main(int argc, char* argv[]) 
{
    // Create a hello.txt file in the same directory of the executable
    HexBuffer buffer = get_hex("hello.txt");

    for (long i = 0; i < buffer.size; i++)
    {
        printf("Hex: %hhx\n", buffer.buffer[i]);
    }

    free_hex(buffer);

    return 0;
}
