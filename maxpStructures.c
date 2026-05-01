typedef struct MaxpTable {
    uint32_t version;
    uint16_t numGlyphs;
    uint16_t maxPoints;
    uint16_t maxContours;
    uint16_t maxCompositPoints;
    uint16_t maxCompositContours;
    uint16_t maxZones;
    uint16_t maxTwilightPoints;
    uint16_t maxStorage;
    uint16_t maxFunctionDefs;
    uint16_t maxInstructionDefs;
    uint16_t maxStackElements;
    uint16_t maxSizeOfInstructions;
    uint16_t maxComponentElements;
    uint16_t maxComponentDepth;
} MaxpTable;

void printfMaxpTable(MaxpTable table){
    printf("------------------------- MAXP table -------------------------\n");
    printf("version: %X\n"
        "numGlyphs: %d\n"
        "maxPoints: %d\n"
        "maxContours: %d\n"
        "maxCompositPoints: %d\n"
        "maxCompositContours: %d\n"
        "maxZones: %d\n"
        "maxTwilightPoints: %d\n"
        "maxStorage: %d\n"
        "maxFunctionDefs: %d\n"
        "maxInstructionDefs: %d\n"
        "maxStackElements: %d\n"
        "maxSizeOfInstructions: %d\n"
        "maxComponentElements: %d\n"
        "maxComponentDepth: %d\n",
        table.version, table.numGlyphs, table.maxPoints,
        table.maxContours, table.maxCompositPoints, table.maxCompositContours,
        table.maxZones, table.maxTwilightPoints, table.maxStorage,
        table.maxFunctionDefs, table.maxInstructionDefs, table.maxStackElements,
        table.maxSizeOfInstructions, table.maxComponentElements, table.maxComponentDepth);
}

void littleToBigEndianMaxpTable(MaxpTable *table){
    table->version = __builtin_bswap32(table->version);
    table->numGlyphs = __builtin_bswap16(table->numGlyphs);
    table->maxPoints = __builtin_bswap16(table->maxPoints);
    table->maxContours = __builtin_bswap16(table->maxContours);
    table->maxZones = __builtin_bswap16(table->maxZones);
    table->maxTwilightPoints = __builtin_bswap16(table->maxTwilightPoints);
    table->maxStorage = __builtin_bswap16(table->maxStorage);
    table->maxFunctionDefs = __builtin_bswap16(table->maxFunctionDefs);
    table->maxStackElements = __builtin_bswap16(table->maxStackElements);
    table->maxSizeOfInstructions = __builtin_bswap16(table->maxSizeOfInstructions);
    table->maxComponentElements = __builtin_bswap16(table->maxComponentElements);
    table->maxComponentDepth = __builtin_bswap16(table->maxComponentDepth);
}