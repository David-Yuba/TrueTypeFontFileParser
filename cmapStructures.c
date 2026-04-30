#ifndef STDINT
#define STDINT
#include <stdint.h>
#endif

#ifndef LOAD_FILE
#define LOAD_FILE
#include "./loadFile.c"
#endif

#ifndef Offset32
#define Offset32 uint32_t
#endif

#ifndef UNICODE_BMP_ONLY
#define UNICODE_PLATFORM 0
#define UNICODE_BMP_ONLY 3
#endif

typedef struct EncodingRecord {
    uint16_t platformID;
    uint16_t encodingID;
    Offset32 subtableOffset; 
} EncodingRecord;

typedef struct Cmap_Header {
    uint16_t version;
    uint16_t numTables;
    EncodingRecord* encodingRecords;
} Cmap_Header;

typedef struct RecordSubtable {
    uint16_t format;
    uint16_t length;
    uint16_t language;
    uint16_t segCountX2;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;
    uint16_t *endCode;
    uint16_t reservedPad;
    uint16_t *startCode;
    uint16_t *idDelta;
    uint16_t *idRangeOffset;
    uint16_t *glyphIdArray;
} RecordSubtable;

void printCmapTable(Cmap_Header cmap) {
    printf("------------------------- Cmap header -------------------------\n");
    printf("Version: %d\n"
        "Number of tables: %d\n", 
        cmap.version, cmap.numTables);
    printf("------------------------ Encoding records ------------------------\n");
    for(int i=0 ; i<cmap.numTables ; i++)
        printf("Platform ID: %d\n"
            "Encoding ID: %d\n"
            "Subtable offset: %d\n",
            cmap.encodingRecords[i].platformID,
            cmap.encodingRecords[i].encodingID,
            cmap.encodingRecords[i].subtableOffset); 
}

int findUnicodeSubtable(Cmap_Header cmap){
    int i=0;

    for (i ; i<cmap.numTables ; i ++){
        switch(cmap.encodingRecords[i].platformID) {
            case UNICODE_PLATFORM:
                if(cmap.encodingRecords[i].encodingID == UNICODE_BMP_ONLY)
                    return i;
                else break;
            default:
                break;
        }
    }

    return -1;
}

void littleToBigEndianCmapTable(Cmap_Header *cmap){
	for (int i=0 ; i<(*cmap).numTables ; i++){
		(*cmap).encodingRecords[i].platformID = __builtin_bswap16((*cmap).encodingRecords[i].platformID);
		(*cmap).encodingRecords[i].encodingID = __builtin_bswap16((*cmap).encodingRecords[i].encodingID);
		(*cmap).encodingRecords[i].subtableOffset = __builtin_bswap32((*cmap).encodingRecords[i].subtableOffset);
	}
}

void printRecordSubtable(RecordSubtable subtable){
    printf("------------------------- Record subtable -------------------------\n");
    printf("Format: %d\n"
        "Length: %d\n"
        "Language: %d\n"
        "segCountX2: %d\n"
        "searchRange: %d\n"
        "entrySelector: %d\n"
        "rangeShift: %d\n",
        subtable.format, subtable.length, subtable.language,
        subtable.segCountX2, subtable.searchRange, subtable.entrySelector,
        subtable.rangeShift);
    printf("endCodes: ");
    for(int i=0 ; i<subtable.segCountX2/2 ; i++){
        printf("%X ", subtable.endCode[i]);
    }
    printf("\nreservedPad: %d\n",
        subtable.reservedPad);
    printf("startCodes: ");
    for(int i=0 ; i<subtable.segCountX2/2 ; i++){
        printf("%X ", subtable.startCode[i]);
    }
    printf("\n");
    printf("idDeltas: ");
    for(int i=0 ; i<subtable.segCountX2/2 ; i++){
        printf("%X ", subtable.idDelta[i]);
    }
    printf("\n"); 
    printf("idRangeOffsets: ");
    for(int i=0 ; i<subtable.segCountX2/2 ; i++){
        printf("%X ", subtable.idRangeOffset[i]);
    }
    printf("\n");
}

void littleToBigEndianCmapSubtable(RecordSubtable *subtable){
    (*subtable).format = __builtin_bswap16((*subtable).format);
    (*subtable).length = __builtin_bswap16((*subtable).length);
    (*subtable).language = __builtin_bswap16((*subtable).language);
    (*subtable).segCountX2 = __builtin_bswap16((*subtable).segCountX2);
    (*subtable).searchRange = __builtin_bswap16((*subtable).searchRange);
    (*subtable).entrySelector = __builtin_bswap16((*subtable).entrySelector);
    (*subtable).rangeShift = __builtin_bswap16((*subtable).rangeShift);  
}

void littleToBigEndianCodes(RecordSubtable *subtable){
    for(int i=0 ; i<subtable->segCountX2/2 ; i++){
        (*subtable).endCode[i] = __builtin_bswap16((*subtable).endCode[i]);
        (*subtable).startCode[i] = __builtin_bswap16((*subtable).startCode[i]);
        (*subtable).idDelta[i] = __builtin_bswap16((*subtable).idDelta[i]);
        (*subtable).idRangeOffset[i] = __builtin_bswap16((*subtable).idRangeOffset[i]);
    }
}

uint16_t get_segment_index(RecordSubtable *subtable, uint16_t charCode){
    for (int i=0 ; i<subtable->segCountX2/2 ; i++){
        if(charCode > subtable->startCode[i] && charCode < subtable->endCode[i]){
            return i;
        }
    }
    return 0;
}