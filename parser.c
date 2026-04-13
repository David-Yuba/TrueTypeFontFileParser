#include<stdbool.h>
//#include "./dictOperator.c"
#include "./file.c"

int main(){
	char filePath[100] = "./Fonts/Helvetica Font Family/Helvetica.ttf";
	BUFFER* fileBuffer = loadFileToBuffer(filePath);

	printBuffer(fileBuffer);

	return 0;
}
