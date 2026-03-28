#ifndef Tag
#include "./otfStructures.c"
#endif

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
}
void printCFFTable(struct CFFTable* table){
	printf("----------------------------- CFF Table ------------------------------\n");
	printf("Version: %d.%d.\n", table->header.major, table->header.minor);
	printf("Header size: %d\n"
		"Absolute offset size: %d\n",
		table->header.hdrSize, table->header.offSize);

	printf("\nName index\n");
	printf("Objects in index: %X\n"
		"Offset element size: %X\n", table->nameIndex.count, table->nameIndex.offSize);
	printf("Offset array: ");
	for(int i=0 ; i<table->nameIndex.count+1 ; i++){
		printf("%X ", table->nameIndex.offset[i]);
	} printf("\n");
	printf("Index data: ");
	for(int i=0 ; i<table->nameIndex.count ; i++){
		int previousOffset;
		if(i == 0) previousOffset = 0;
		else previousOffset = table->nameIndex.offset[i];

		for(int j=0 ; j<table->nameIndex.offset[i+1] - 1 ; j++){
			printf("%c", table->nameIndex.data[j + previousOffset]);
		}
	}

	printf("\n\nTop Dict Index\n");
	printf("# of objects in index: %X\n"
		"Offset element size: %X\n",table->topDictIndex.count, table->topDictIndex.offSize);
	printf("Offset array: ");
	for(int i=0 ; i<table->topDictIndex.count+1 ; i++){
		printf("%X ", table->topDictIndex.offset[i]);
	} printf("\n");
	printf("Index data: ");
	for(int i=0 ; i<table->topDictIndex.count; i++){
		int previousOffset;
		if(i == 0) previousOffset = 0;
		else previousOffset = table->topDictIndex.offset[i];

		for(int j=0 ; j<table->topDictIndex.offset[i+1] - 1 ; j++){
			printf("%X ", table->topDictIndex.data[j + previousOffset]);
		}
	}
}