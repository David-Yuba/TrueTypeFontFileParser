#ifndef INTTYPES
#define INTTYPES
#include<inttypes.h>
#endif

#pragma pack(1)

typedef struct GlyphHeader {
    uint16_t numberOfContours;
    uint16_t xMin;
    uint16_t yMin;
    uint16_t xMax;
    uint16_t yMax;
} GlyphHeader;

typedef struct TrueTypeOutline8 {
    uint16_t *endPtsOfCountours;
    uint16_t instructionLength;
    uint8_t *instructions;
    uint8_t *flags;
    uint8_t *xCoordinates;
    uint8_t *yCoordinates;
} TrueTypeOutline8;

typedef struct TrueTypeOutline16 {
    uint16_t *endPtsOfCountours;
    uint16_t instructionLength;
    uint8_t *instructions;
    uint8_t *flags;
    int16_t *xCoordinates;
    int16_t *yCoordinates;
} TrueTypeOutline16;

typedef struct Glyph8 {
    GlyphHeader header;
    TrueTypeOutline8 fontOutline;
} Glyph8;

typedef struct Glyph16 {
    GlyphHeader header;
    TrueTypeOutline16 fontOutline;
} Glyph16;

typedef struct LocaTable16{
    uint16_t *offsets;
} LocaTable16;

typedef struct LocaTable32{
    uint32_t *offsets;
} LocaTable32;

void littleToBigEndianLocaTable16(LocaTable16 *table, uint16_t numGlyphs){
    for(int i=0 ; i<numGlyphs+1 ; i++){
        table->offsets[i] = __builtin_bswap16(table->offsets[i]);
    }
}
void littleToBigEndianLocaTable32(LocaTable32 *table, uint32_t numGlyphs){
    for(int i=0 ; i<numGlyphs+1 ; i++){
        table->offsets[i] = __builtin_bswap32(table->offsets[i]);
    }
}
void printfLocaTable16(LocaTable16 table, uint16_t numGlyphs){
    for(int i=0 ; i<numGlyphs+1 ; i++){
        printf("%d\t", table.offsets[i]);
    }
    printf("\n");
}
void printfLocaTable32(LocaTable32 table, uint32_t numGlyphs){
    for(int i=0 ; i<numGlyphs+1 ; i++){
        printf("%d\t", table.offsets[i]);
    }
    printf("\n");
}