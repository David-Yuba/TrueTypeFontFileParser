#define Tag uint8_t
#define Offset32 uint32_t

struct TableRecord {
	Tag tableTag[3];
	uint32_t checksum;
	Offset32 offset;
	uint32_t length;
};

#define Card8 uint8_t
#define Card16 uint16_t
#define Offset uint8_t
#define OffSize uint8_t

struct IndexFormat {
	Card16 count;
	OffSize offSize;
	Offset offset[];
};

struct Header {
	Card8 major;
	Card8 minor;
	Card8 hdrSize;
	OffSize offSize;
};

struct NameIndex {
	struct IndexFormat index;
	char data[];
};

struct CFFTable {
	struct Header header;
	struct NameIndex nameIndex;
//	struct TopDictIndex topDictIndex;
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
	struct TableRecord *tableRecords;
	struct CFFTable *cffTable;
};


