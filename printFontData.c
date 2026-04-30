#ifndef Tag
#include "./otfStructures.c"
#endif

void printFontInfo(struct TableDirectory font){
	printf(
		"--------------------------- Table Directory ---------------------------\n"
		"sfntVersion: %08X\n"
		"numTables: %d\n"
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