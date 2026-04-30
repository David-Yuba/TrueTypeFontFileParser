#include<inttypes.h>
#include<stdbool.h>
#include<string.h>
#ifndef LOAD_FILE
#define LOAD_FILE
#include "./loadFile.c"
#endif
#include "./otfStructures.c"
#include "./dictOperator.c"
#include "./printFontData.c"
#include "./cmapStructures.c"

void* loadTableToBuffer(FILE* fontFile, struct TableDirectory* font, char tableName[], void* tableDataBuffer){
	if(tableDataBuffer != NULL) free(tableDataBuffer);
	int tableNumber = returnTableNumber(font, tableName);
	struct TableRecord tableData = font->tableRecords[tableNumber];

	tableDataBuffer = malloc(tableData.length);
	fseek(fontFile, tableData.offset, SEEK_SET);
	fread(tableDataBuffer, 1, tableData.length, fontFile);
	//printBuffer(tableDataBuffer, tableData.length);

	return tableDataBuffer;
}

void parseCmap(void* tableBuffer){
	Cmap_Header cmapTable;
	
	cmapTable.version = __builtin_bswap16(((uint16_t*)tableBuffer)[0]);
	cmapTable.numTables = __builtin_bswap16(((uint16_t*)tableBuffer)[1]);
	cmapTable.encodingRecords = malloc(cmapTable.numTables * sizeof(EncodingRecord));
	memcpy(cmapTable.encodingRecords, ((char*)tableBuffer) + 4, cmapTable.numTables * sizeof(EncodingRecord));
	littleToBigEndianCmapTable(&cmapTable);
	uint32_t unicodeTableOffset = cmapTable.encodingRecords[findUnicodeSubtable(cmapTable)].subtableOffset;

	RecordSubtable unicodeBitMapSubtable;
	memcpy(&unicodeBitMapSubtable, (char *)tableBuffer + unicodeTableOffset, sizeof(RecordSubtable));
	littleToBigEndianCmapSubtable(&unicodeBitMapSubtable);

	unicodeBitMapSubtable.endCode = malloc(sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2);
	memcpy(unicodeBitMapSubtable.endCode, (char *)tableBuffer + unicodeTableOffset + 14, sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2);

	memcpy(&(unicodeBitMapSubtable.reservedPad), (char *)tableBuffer + unicodeTableOffset + 14 + sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2, sizeof(uint16_t));

	unicodeBitMapSubtable.startCode = malloc(sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2);
	memcpy(unicodeBitMapSubtable.startCode, (char *)tableBuffer + unicodeTableOffset + 16 + sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2, sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2);

	unicodeBitMapSubtable.idDelta = malloc(sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2);
	memcpy(unicodeBitMapSubtable.idDelta, (char *)tableBuffer + unicodeTableOffset + 16 + 2 * sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2, sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2);

	unicodeBitMapSubtable.idRangeOffset = malloc(sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2);
	memcpy(unicodeBitMapSubtable.idRangeOffset, (char *)tableBuffer + unicodeTableOffset + 16 + 3 * sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2, sizeof(uint16_t) * unicodeBitMapSubtable.segCountX2 / 2);

	littleToBigEndianCodes(&unicodeBitMapSubtable);

	printCmapTable(cmapTable);
	printRecordSubtable(unicodeBitMapSubtable);

	uint16_t charsegment = get_segment_index(&unicodeBitMapSubtable, '7');
	//uint16_t glyphid = *(unicodeBitMapSubtable.idRangeOffset[charsegment]/2 + ('7' - unicodeBitMapSubtable.startCode[charsegment]) + &unicodeBitMapSubtable.idRangeOffset[charsegment]);
	uint16_t glyphid = '7' + unicodeBitMapSubtable.idDelta[charsegment];

	printf("\nGlyphID: %d\n"
		"SegmentIndex: %d\n", 
		glyphid, charsegment);

	free(unicodeBitMapSubtable.endCode);
	free(unicodeBitMapSubtable.startCode);
	free(unicodeBitMapSubtable.idDelta);
	free(unicodeBitMapSubtable.idRangeOffset);
	free(cmapTable.encodingRecords);
}

int main(){
	char filePath[100] = "./Fonts/Helvetica Font Family/Helvetica.ttf";

	struct TableDirectory* font = malloc(sizeof(struct TableDirectory));
	void* tableDataBuffer = NULL;

	FILE* fontFile = fopen(filePath,"rb");

	fread(&font->sfntVersion, sizeof(uint32_t), 1, fontFile);
	fread(&font->numTables, sizeof(uint16_t), 4, fontFile);
	font->tableRecords = malloc(sizeof(struct TableRecord)*font->numTables);
	fread(font->tableRecords, sizeof(struct TableRecord),font->numTables, fontFile);
	littleToBigEndian(font);

	tableDataBuffer = loadTableToBuffer(fontFile, font, "cmap", tableDataBuffer);
	printFontInfo(*font);
	parseCmap(tableDataBuffer);

	free(tableDataBuffer);
	free(font->tableRecords);
	free(font);
	fclose(fontFile);
	return 0;
}
