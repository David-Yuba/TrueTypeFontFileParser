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
#include "./maxpStructures.c"
#include "./horizontalTables.c"
#include "./headStructures.c"
#include "./glyphStructures.c"

void* loadTableToBuffer(FILE *fontFile, struct TableDirectory *font, char tableName[], void *tableDataBuffer, bool p){
	if(tableDataBuffer != NULL) free(tableDataBuffer);
	int tableNumber = returnTableNumber(font, tableName);
	struct TableRecord tableData = font->tableRecords[tableNumber];

	tableDataBuffer = malloc(tableData.length);
	fseek(fontFile, tableData.offset, SEEK_SET);
	fread(tableDataBuffer, 1, tableData.length, fontFile);
	if(p) printBuffer(tableDataBuffer, tableData.length);

	return tableDataBuffer;
}

void parseCmap(void *tableBuffer){
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

	//printCmapTable(cmapTable);
	//printRecordSubtable(unicodeBitMapSubtable);

	uint16_t charsegment = get_segment_index(&unicodeBitMapSubtable, '7');
	//uint16_t glyphid = *(unicodeBitMapSubtable.idRangeOffset[charsegment]/2 + ('7' - unicodeBitMapSubtable.startCode[charsegment]) + &unicodeBitMapSubtable.idRangeOffset[charsegment]);
	uint16_t glyphid = '7' + unicodeBitMapSubtable.idDelta[charsegment];

//	printf("\nGlyphID: %d\n"
//		"SegmentIndex: %d\n", 
//		glyphid, charsegment);

	free(unicodeBitMapSubtable.endCode);
	free(unicodeBitMapSubtable.startCode);
	free(unicodeBitMapSubtable.idDelta);
	free(unicodeBitMapSubtable.idRangeOffset);
	free(cmapTable.encodingRecords);
}

void parseMaxp(void *tableBuffer, MaxpTable *maxpTable){
	
	memcpy(maxpTable, tableBuffer, sizeof(MaxpTable));
	littleToBigEndianMaxpTable(maxpTable);
	//printfMaxpTable(*maxpTable);
}

void parseHorizontalHeader(void *tableBuffer, HorizontalHeader *horizontalHeader) {
	memcpy(horizontalHeader, tableBuffer, sizeof(HorizontalHeader));
	littleToBigEndianHorizontalHeader(horizontalHeader);
	//printfHorizontalHeader(*horizontalHeader);
}

void parseHorizontalMetrics(void *tableBuffer, MaxpTable maxpTable,HorizontalHeader horizontalHeader){
	HorizontalMetrics horizontalMetrics;
	horizontalMetrics.hMetrics = malloc(sizeof(LongHorMetric) * horizontalHeader.numberOfHMetrics);
	horizontalMetrics.leftSideBearings = malloc(sizeof(int16_t) * (maxpTable.numGlyphs - horizontalHeader.numberOfHMetrics));

	memcpy(horizontalMetrics.hMetrics, tableBuffer, sizeof(LongHorMetric) * horizontalHeader.numberOfHMetrics);
	memcpy(horizontalMetrics.leftSideBearings, (LongHorMetric *)tableBuffer + horizontalHeader.numberOfHMetrics, sizeof(int16_t) * (maxpTable.numGlyphs - horizontalHeader.numberOfHMetrics));
	littleToBigEndianHorizontalMetrics(maxpTable.numGlyphs, horizontalHeader, horizontalMetrics);

	//printfHorizontalMetrics(maxpTable.numGlyphs, horizontalHeader, horizontalMetrics);
	
	free(horizontalMetrics.hMetrics);
	free(horizontalMetrics.leftSideBearings);
}

void parseHeadTable(void *tableBuffer, HeadTable *table){
	memcpy(table, tableBuffer, sizeof(HeadTable));
	littleToBigEndianHeadTable(table);
	//printfHeadTable(*table);
}

void parseLocaTable16(void *tableBuffer, MaxpTable maxpTable, LocaTable16 *table){
	table->offsets = malloc(sizeof(uint16_t) * maxpTable.numGlyphs);
	memcpy(table->offsets, tableBuffer, sizeof(LocaTable16) * sizeof(maxpTable.numGlyphs));
	littleToBigEndianLocaTable16(table, maxpTable.numGlyphs);
	printf("%d\n", maxpTable.numGlyphs);
	//printfLocaTable16(*table, maxpTable.numGlyphs);
}
void parseLocaTable32(void *tableBuffer, MaxpTable maxpTable, LocaTable32 *table){
	table->offsets = malloc(sizeof(uint32_t) * maxpTable.numGlyphs);
	memcpy(table->offsets, tableBuffer, sizeof(LocaTable32) * maxpTable.numGlyphs);
	littleToBigEndianLocaTable32(table, maxpTable.numGlyphs);
	//printfLocaTable32(*table, maxpTable.numGlyphs);
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

	tableDataBuffer = loadTableToBuffer(fontFile, font, "cmap", tableDataBuffer, 0);
	//printFontInfo(*font);
	parseCmap(tableDataBuffer);

	tableDataBuffer = loadTableToBuffer(fontFile, font, "maxp", tableDataBuffer, 0);
	MaxpTable maxpTable;
	parseMaxp(tableDataBuffer, &maxpTable);

	tableDataBuffer = loadTableToBuffer(fontFile, font, "hhea", tableDataBuffer, 0);
	HorizontalHeader horizontalHeader;
	parseHorizontalHeader(tableDataBuffer, &horizontalHeader);
	
	tableDataBuffer = loadTableToBuffer(fontFile, font, "hmtx", tableDataBuffer, 0);
	parseHorizontalMetrics(tableDataBuffer, maxpTable, horizontalHeader);

	HeadTable headTable;
	tableDataBuffer = loadTableToBuffer(fontFile, font, "head", tableDataBuffer, 0);
	parseHeadTable(tableDataBuffer, &headTable);

	LocaTable16 locaTable16;
	locaTable16.offsets = NULL;
	LocaTable32 locaTable32;
	locaTable32.offsets = NULL;
	tableDataBuffer = loadTableToBuffer(fontFile, font, "loca", tableDataBuffer, 0);
	if(!headTable.indexToLocFormat)
		parseLocaTable16(tableDataBuffer, maxpTable, &locaTable16);
	else
		parseLocaTable32(tableDataBuffer, maxpTable, &locaTable32);

	tableDataBuffer = loadTableToBuffer(fontFile, font, "glyf", tableDataBuffer, 0);

	free(tableDataBuffer);
	free(font->tableRecords);
	free(font);
	fclose(fontFile);
	return 0;
}
