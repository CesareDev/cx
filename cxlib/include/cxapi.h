#ifndef CXAPI_H
#define CXAPI_H

typedef struct {
    unsigned char* buffer;
    unsigned long size;
} HexBuffer;

// Remember to free the memory
HexBuffer get_hex(const char* filename);
void free_hex(HexBuffer buffer);

#endif // !CXAPI_H
