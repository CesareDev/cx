#ifndef CXAPI_H
#define CXAPI_H

typedef struct {
    unsigned char* buffer;
    unsigned long size;
} HexData;

// Remember to free the memory
HexData get_hex(const char* filename);
void free_hex(HexData data);

#endif // !CXAPI_H
