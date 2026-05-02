#ifndef INTTYPES
#define INTTYPES
#include<inttypes.h>
#endif
#pragma pack(1)

typedef struct HeadTable {
    uint16_t majorVersion;
    uint16_t minorVersion;
    uint32_t fontRevision;
    uint32_t checksumAdjustment;
    uint32_t magicNumber;
    uint16_t flags;
    uint16_t unitsPerEm;
    int64_t created;
    int64_t modified;
    int16_t xMin;
    int16_t yMin;
    int16_t xMax;
    int16_t yMax;
    uint16_t macStyle;
    uint16_t lowestRecPPEM;
    int16_t fontDirectionHint;
    int16_t indexToLocFormat;
    int16_t glyphDataFormat;
} HeadTable;

void printfHeadTable(HeadTable table){ 
    printf("------------------------- Head table -------------------------\n");
    printf("majorVersion: %d\n"
        "minorVersion: %d\n"
        "fontRevision: %f\n"
        "checksumAdjustment: %d\n"
        "magicNumber: %d\n"
        "flags: %d\n"
        "unitsPerEm: %d\n"
        "created: %lli\n"
        "modified: %lli\n"
        "xMin: %d\n"
        "yMin: %d\n"
        "xMax: %d\n"
        "yMax: %d\n"
        "macStyle: %d\n"
        "lowestRecPPEM: %d\n"
        "fontDirectionHint: %d\n"
        "indexToLocFormat: %d\n"
        "glyphDataFormat: %d\n",
        table.majorVersion, table.minorVersion, table.fontRevision, 
        table.checksumAdjustment, table.magicNumber, table.flags,
        table.unitsPerEm, table.created, table.modified,
        table.xMin, table.yMin, table.xMax, table.yMax,
        table.macStyle, table.lowestRecPPEM, table.fontDirectionHint,
        table.indexToLocFormat, table.glyphDataFormat);
}

void littleToBigEndianHeadTable(HeadTable *table){
    table->majorVersion = __builtin_bswap16(table->majorVersion);
    table->minorVersion = __builtin_bswap16(table->minorVersion); 
    table->fontRevision = __builtin_bswap32(table->fontRevision);
    table->checksumAdjustment = __builtin_bswap32(table->checksumAdjustment);
    table->magicNumber = __builtin_bswap32(table->magicNumber);
    table->flags = __builtin_bswap16(table->flags);
    table->unitsPerEm = __builtin_bswap16(table->unitsPerEm);
    table->created = __builtin_bswap64(table->created);
    table->modified = __builtin_bswap64(table->modified);
    table->xMin = __builtin_bswap16(table->xMin);
    table->yMin = __builtin_bswap16(table->yMin);
    table->xMax = __builtin_bswap16(table->xMax);
    table->yMax = __builtin_bswap16(table->yMax);
    table->macStyle = __builtin_bswap16(table->macStyle);
    table->lowestRecPPEM = __builtin_bswap16(table->lowestRecPPEM);
    table->fontDirectionHint = __builtin_bswap16(table->fontDirectionHint);
    table->indexToLocFormat = __builtin_bswap16(table->indexToLocFormat);
    table->glyphDataFormat = __builtin_bswap16(table->glyphDataFormat);
}