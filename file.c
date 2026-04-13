#ifndef STDIO_H
#define STDIO_H
#include<stdio.h>
#endif
#ifndef STDLIB_H
#define STDLIB_H
#include<stdlib.h>
#endif

typedef struct {
    int size;
    char *data;
} BUFFER;

int getFileSize(FILE *f) {
    fseek(f, 0, SEEK_END);

    int size = ftell(f);

    fseek(f, 0, SEEK_SET);
    return size;
}

BUFFER* loadFileToBuffer(char filePath[]) {
    FILE *f = fopen(filePath, "rb");
    int sizeOfFile = getFileSize(f);

    BUFFER* buffer;

    buffer->size = sizeOfFile;
    buffer->data = malloc(sizeOfFile);

    fread(buffer->data, 1, sizeOfFile, f);

    return buffer;
}

void printBuffer(BUFFER *buffer){
    for(int i=0 ; i<buffer->size ; i++){
        if(i % 4 == 0) printf("\n%p\t", buffer->data + i);
        printf("%X\t", (unsigned char)buffer->data[i]);
    }
}

