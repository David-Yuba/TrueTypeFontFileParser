#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFFER_SIZE 1024
#define BUFFER_TYPE uint8_t

void printBuffer(BUFFER_TYPE *buffer, int size){
	for (int i=0 ; i<size ; i++){
		if (i % 4 == 0) printf("\n%p\t", buffer + i);
		printf("%8X ", buffer[i]);
	}
}

void printBufferToFile(BUFFER_TYPE *buffer, int size){
	FILE *f = fopen("FontMemory.txt","w");
	for (int i=0 ; i<size ; i++){
		if (i % 4 == 0) fprintf(f, "\n%p\t", buffer + i);
		fprintf(f, "%8X ", buffer[i]);
	}
}

int getFileSize(FILE *file){
	fseek(file, 0L, SEEK_END);
	int size = (int)ftell(file);
	printf("\nFile size = %d\n", size);

	fseek(file, 0L, SEEK_SET);
	return size;
}

int loadFileToBuffer(uint8_t **buffer, char filePath[]){
	int i = 0;
	FILE* file = fopen(filePath,"rb");
	*buffer = realloc(*buffer, getFileSize(file) * sizeof(uint8_t));

	int ch;
	while((ch = fgetc(file)) != EOF){
		(*buffer)[i++] = ch;
	}
	if (feof(file))
		puts("EOF indicator set");
	if (ferror(file))
		puts("Error indicator set");
	fclose(file);

	return i;
}
