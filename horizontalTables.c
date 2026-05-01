#ifndef INTTYPES
#define INTTYPES
#include<inttypes.h>
#endif

typedef struct HorizontalHeader {
    uint16_t majorVersion;
    uint16_t minorVersion;
    int16_t ascender;
    int16_t descender;
    int16_t lineGap;
    uint16_t advanceWidthMax;
    int16_t minLeftSideBearing;
    int16_t minRightSideBearing;
    int16_t xMaxExtent;
    int16_t caretSlopeRise;
    int16_t caretSlopeRun;
    int16_t caretOffset;
    int16_t reserved1;
    int16_t reserved2;
    int16_t reserved3;
    int16_t reserved4; 
    int16_t metricDataFormat;
    uint16_t numberOfHMetrics;
} HorizontalHeader;

typedef struct LongHorMetric {
    uint16_t advanceWidth;
    int16_t lsb;
} LongHorMetric;

typedef struct HorizontalMetrics {
    LongHorMetric *hMetrics;
    int16_t *leftSideBearings; 
} HorizontalMetrics;

void printfHorizontalHeader(HorizontalHeader header) {
    printf("------------------------- Horizontal table header -------------------------\n");
    printf("majorVersion: %d\n"
        "minorVersion: %d\n"
        "ascender: %d\n"
        "descender: %d\n"
        "lineGap: %d\n"
        "advanceWidthMax: %d\n"
        "minLeftSideBearing: %d\n"
        "minRightSideBearing: %d\n"
        "xMaxExtent: %d\n"
        "caretSlopeRise: %d\n"
        "caretSlopeRun: %d\n"
        "caretOffset: %d\n"
        "reserved: %d\n"
        "reserved: %d\n"
        "reserved: %d\n"
        "reserved: %d\n"
        "metricDataFormat: %d\n"
        "numberOfHMetrics: %d\n",
        header.majorVersion, header.minorVersion, header.ascender,
        header.descender, header.lineGap, header.advanceWidthMax,
        header.minLeftSideBearing, header.minRightSideBearing, header.xMaxExtent,
        header.caretSlopeRise, header.caretSlopeRun, header.caretOffset,
        header.reserved1, header.reserved2, header.reserved3, header.reserved4,
        header.metricDataFormat, header.numberOfHMetrics);
}

void littleToBigEndianHorizontalHeader(HorizontalHeader *header){
    header->majorVersion = __builtin_bswap16(header->majorVersion);
    header->minorVersion = __builtin_bswap16(header->minorVersion);
    header->ascender = __builtin_bswap16(header->ascender);
    header->descender = __builtin_bswap16(header->descender);
    header->lineGap = __builtin_bswap16(header->lineGap);
    header->advanceWidthMax = __builtin_bswap16(header->advanceWidthMax);
    header->minLeftSideBearing = __builtin_bswap16(header->minLeftSideBearing);
    header->minRightSideBearing = __builtin_bswap16(header->minRightSideBearing);
    header->xMaxExtent = __builtin_bswap16(header->xMaxExtent);
    header->caretSlopeRise = __builtin_bswap16(header->caretSlopeRise);
    header->caretSlopeRun = __builtin_bswap16(header->caretSlopeRun);
    header->caretOffset = __builtin_bswap16(header->caretOffset);
    header->metricDataFormat = __builtin_bswap16(header->metricDataFormat);
    header->numberOfHMetrics = __builtin_bswap16(header->numberOfHMetrics);
}

void printfHorizontalMetrics(uint16_t glyphNumber, HorizontalHeader header, HorizontalMetrics metrics){
    printf("------------------------- Horizontal metrics hMetrics -------------------------\n");
    for(int i=0 ; i<header.numberOfHMetrics ; i++){
        printf("advanceWidth: %d\n"
            "lsb: %d\n",
            metrics.hMetrics[i].advanceWidth, metrics.hMetrics[i].lsb);
    }
    printf("------------------------- Horizontal metrics leftSideBearings -------------------------\n");
    for(int i=0 ; i<glyphNumber-header.numberOfHMetrics ; i++){
        printf("lsb: %d\n",
            metrics.leftSideBearings[i]);
    }
}

void littleToBigEndianHorizontalMetrics(uint16_t glyphNumber, HorizontalHeader header, HorizontalMetrics metrics) {
    for(int i=0 ; i<header.numberOfHMetrics ; i++){
        metrics.hMetrics[i].advanceWidth = __builtin_bswap16(metrics.hMetrics[i].advanceWidth);
        metrics.hMetrics[i].lsb = __builtin_bswap16(metrics.hMetrics[i].lsb);
    }

    for(int i=0 ; i<glyphNumber-header.numberOfHMetrics ; i++){
        metrics.leftSideBearings[i] = __builtin_bswap16(metrics.leftSideBearings[i]);
    }
}