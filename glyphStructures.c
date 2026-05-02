#ifndef INTTYPES
#define INTTYPES
#include<inttypes.h>
#endif

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
} Glyph;

typedef struct Glyph16 {
    GlyphHeader header;
    TrueTypeOutline16 fontOutline;
} Glyph;

typedef struct LocaTable16{
    uint16_t *offsets;
} LocaTable;

typedef struct LocaTable32{
    uint32_t *offsets;
} LocaTable;