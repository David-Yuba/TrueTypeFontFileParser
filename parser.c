#include<inttypes.h>
#include<stdbool.h>
#include "./loadFile.c"
#include "./otfStructures.c"
#include "./dictOperator.c"
#include "./printFontData.c"

void decodeTopDictData(struct TopDictIndex dict){

}

struct CFFTable* loadCFFTable(struct TableDirectory *dir, FILE *file){
	struct CFFTable *table = malloc(sizeof(struct CFFTable));
	uint16_t cffTable = returnTableNumber(dir,"CFF");
	struct TableRecord CFFTable = dir->tableRecords[cffTable];
	fseek(file, CFFTable.offset, SEEK_SET);

	fread(table, sizeof(struct CFFHeader), 1, file);
	fread(&table->nameIndex, sizeof(Card16) + sizeof(OffSize), 1, file);
	table->nameIndex.count = __builtin_bswap16(table->nameIndex.count);
	//littleToBigEndianCFFTable(table);

	table->nameIndex.offset = malloc(sizeof(uint8_t)*table->nameIndex.offSize*table->nameIndex.count + sizeof(uint8_t));
	fread(table->nameIndex.offset, sizeof(uint8_t), table->nameIndex.count + 1, file);
	{int indexDataSize = 0;
	for(int i=0 ; i<table->nameIndex.count ; i++) 
		indexDataSize += table->nameIndex.offset[i+1]-1;
	table->nameIndex.data = malloc(sizeof(uint8_t)*indexDataSize);
	fread(table->nameIndex.data, sizeof(Card8), indexDataSize, file);}

	fread(&table->topDictIndex, sizeof(Card16) + sizeof(OffSize), 1, file);
	table->topDictIndex.count = __builtin_bswap16(table->topDictIndex.count);
	table->topDictIndex.offset = malloc(sizeof(uint8_t)*table->topDictIndex.offSize*table->topDictIndex.count + sizeof(uint8_t)*table->topDictIndex.offSize);
	fread(table->topDictIndex.offset, sizeof(uint8_t)*table->topDictIndex.offSize, table->topDictIndex.count + 1, file);
	for(int i=0 ; i<table->topDictIndex.count + 1 ; i++)
		table->topDictIndex.offset[i] = __builtin_bswap16(table->topDictIndex.offset[i]);
	{
		int indexDataSize = 0;
		for(int i=0 ; i<table->topDictIndex.count ; i++) 
			indexDataSize += table->topDictIndex.offset[i+1]-1;
		table->topDictIndex.data = malloc(sizeof(uint8_t)*indexDataSize);
		fread(table->topDictIndex.data, sizeof(Card8), indexDataSize, file);
	}

	printf("\n");
	decodeTopDictData(table->topDictIndex);

	return table;
}

int main(){
	char filePath[100] = "./Fonts/Helvetica Font Family/Helvetica.ttf";

	struct TableDirectory* font = malloc(sizeof(struct TableDirectory));
	struct CFFTable* cffTable;
	FILE* fontFile = fopen(filePath,"rb");

	fread(&font->sfntVersion, sizeof(uint32_t), 1, fontFile);
	fread(&font->numTables, sizeof(uint16_t), 4, fontFile);
	font->tableRecords = malloc(sizeof(struct TableRecord)*font->numTables);
	fread(font->tableRecords, sizeof(struct TableRecord),font->numTables, fontFile);
	littleToBigEndian(font);

	cffTable = loadCFFTable(font, fontFile);

	printFontInfo(*font);
	//printCFFTable(cffTable);

	free(cffTable->nameIndex.offset);
	free(cffTable->nameIndex.data);
	free(cffTable->topDictIndex.offset);
	free(cffTable->topDictIndex.data);
	free(cffTable);
	free(font->tableRecords);
	free(font);
	fclose(fontFile);
	return 0;
}
