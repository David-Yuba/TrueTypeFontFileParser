#include<inttypes.h>
#include "./loadFile.c"
#include "./otfStructures.c"

void printFontInfo(struct TableDirectory font){
	printf(
		"--------------------------- Table Directory ---------------------------\n"
		"sfntVersion: %X\n"
		"numTables: %X\n"
		"searchRange: %X\n"
		"entrySelector: %X\n"
		"rangeShift: %X\n",
		font.sfntVersion, font.numTables, font.searchRange, font.entrySelector, font.rangeShift);
	printf("---------------------------- Table Records ----------------------------\n");
	for (int i=0 ; i<font.numTables ; i++){
		printf(
			"\nTable %d.\n"
			"Tag: %c%c%c%c\n"
			"Checksum: %X\n"
			"Offset: %X\n"
			"Length: %X\n",
			i+1,font.tableRecords[i].tableTag[0], font.tableRecords[i].tableTag[1], font.tableRecords[i].tableTag[2], font.tableRecords[i].tableTag[3],
			font.tableRecords[i].checksum, font.tableRecords[i].offset, font.tableRecords[i].length);
	}
	printf("----------------------------- CFF Table ------------------------------\n");
	printf("Version: %d.%d.\n", (*font.cffTable).header.major, (*font.cffTable).header.minor);
	printf("Header size: %d\n"
			"Absolute offset size: %d\n",
			(*font.cffTable).header.hdrSize, (*font.cffTable).header.offSize);
}

void littleToBigEndian(struct TableDirectory *font){
	(*font).sfntVersion = __builtin_bswap32((*font).sfntVersion);
	(*font).numTables = __builtin_bswap16((*font).numTables);
	(*font).searchRange = __builtin_bswap16((*font).searchRange);
	(*font).entrySelector = __builtin_bswap16((*font).entrySelector);
	(*font).rangeShift = __builtin_bswap16((*font).rangeShift);
	for (int i=0 ; i<(*font).numTables ; i++){
		(*font).tableRecords[i].checksum = __builtin_bswap32((*font).tableRecords[i].checksum);
		(*font).tableRecords[i].offset = __builtin_bswap32((*font).tableRecords[i].offset);
		(*font).tableRecords[i].length = __builtin_bswap32((*font).tableRecords[i].length);
	}
}

int main(){
	char filePath[100] = "./Fonts/HelveticaRegular/Helvetica Regular.otf";
	BUFFER_TYPE *fileBuffer = malloc(BUFFER_SIZE*sizeof(BUFFER_TYPE));

	struct TableDirectory font;

	int fileSize = loadFileToBuffer(&fileBuffer, filePath);

	font.sfntVersion = *(uint32_t *) fileBuffer;
	font.numTables = *(uint16_t *) (fileBuffer + 4);
	font.searchRange = *(uint16_t *) (fileBuffer + 6);
	font.entrySelector = *(uint16_t *) (fileBuffer + 8);
	font.rangeShift = *(uint16_t *) (fileBuffer + 10);
	font.tableRecords = (struct TableRecord *) (fileBuffer + 12);

	littleToBigEndian(&font);

	font.cffTable = (struct CFFTable *) (fileBuffer + font.tableRecords[0].offset);


//	printBufferToFile(fileBuffer, fileSize);
	printFontInfo(font);

	free(fileBuffer);
	return 0;
}
