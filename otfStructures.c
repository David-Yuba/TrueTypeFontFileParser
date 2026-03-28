#define Tag uint8_t
#define Offset32 uint32_t
#define SID uint16_t

struct TableRecord {
	Tag tableTag[4];
	uint32_t checksum;
	Offset32 offset;
	uint32_t length;
};

#define Card8 uint8_t
#define Card16 uint16_t
#define Offset uint8_t
#define Offset16 uint16_t
#define OffSize uint8_t

struct NameIndex {
	Card16 count;
	OffSize offSize;
	Offset *offset;
	Card8 *data;
};

struct TopDictData {
	SID version;
	SID Notice;
	SID Copyright;
	SID FullName;
	SID FamilyName;
	SID Weight;
};

struct TopDictIndex {
	Card16 count;
	OffSize offSize;
	Offset16 *offset;
	Card8 *data;
};

struct CFFHeader {
	Card8 major;
	Card8 minor;
	Card8 hdrSize;
	OffSize offSize;
};

struct CFFTable {
	struct CFFHeader header;
	struct NameIndex nameIndex;
	struct TopDictIndex topDictIndex;
//	struct StringIndex stringIndex;
//	struct GlobalSubrIndex globalSubrIndex;
//	struct Encodings encodings;
//	struct Charset charset;
//	struct FDSelect fdSelect;
//	struct CharStringsIndex charStringsIndex;
//	struct FontDictIndex fontDictIndex;
//	struct PrivateDict privateDict;
//	struct LocalSubrIndex localSubrIndex;
//	struct CopyRight copyright;
};

struct TableDirectory {
	uint32_t sfntVersion;
	uint16_t numTables;
	uint16_t searchRange;
	uint16_t entrySelector;
	uint16_t rangeShift;
	struct TableRecord* tableRecords;
};

uint16_t returnTableNumber(struct TableDirectory *dir, char tableName[]){
	uint16_t tableNumber;
	for(int i=0 ; i<dir->numTables ; i++){	
		int foundFlag = 1;
		for(int j=0 ; j<3 ; j++){
			if((dir->tableRecords)[i].tableTag[j] == tableName[j]) continue;
			else {
				foundFlag = 0;
				break;
			}
		}	
		if (foundFlag == 1){
			tableNumber = i;
			break;
		}
	}
	return tableNumber;
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

void littleToBigEndianCFFTable(struct CFFTable* table){
}
