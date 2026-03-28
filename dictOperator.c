#include<stddef.h>
#include<inttypes.h>

#define DICTOPERAND_VERSION 0
#define DICTOPERAND_NOTICE 1
#define DICTOPERAND_FULLNAME 2
#define DICTOPERAND_FAMILYNAME 3
#define DICTOPERAND_WEIGHT 4
#define DICTOPERAND_FONTBBOX 5
#define DICTOPERAND_UNIQUEID 13
#define DICTOPERAND_XUID 14
#define DICTOPERAND_CHARSET 15
#define DICTOPERAND_ENCODING 16
#define DICTOPERAND_CHARSTRINGS 17
#define DICTOPERAND_PRIVATE 18

#define DICTOPERAND_DOUBLEBYTE 12
#define DICTOPERAND_COPYRIGHT 0
#define DICTOPERAND_ISFIXEDPITCH 1
#define DICTOPERAND_ITALICANGLE 2
#define DICTOPERAND_UNDERLINEPOSITION 3
#define DICTOPERAND_UNDERLINETHICKNESS 4
#define DICTOPERAND_PAINTTYPE 5
#define DICTOPERAND_CHARSTRINGTYPE 6
#define DICTOPERAND_FONTMATRIX 7
#define DICTOPERAND_STROKEWIDTH 8
#define DICTOPERAND_SYNTHETICBASE 20
#define DICTOPERAND_POSTSCRIPT 21
#define DICTOPERAND_BASEFONTNAME 22
#define DICTOPERAND_BASEFONTBLEND 23

#define EMPTY 100
struct linkedListHeader {
    void *previousElement;
    void *nextElement;
};
struct linkedListValues {
    int operator;
    void *operand;
};
struct dictLL {
    struct linkedListHeader header;
    struct linkedListValues values;
};

struct dictLL *initDictLL(){
    struct dictLL *list = malloc(sizeof(struct dictLL));
    list->header.previousElement = NULL;
    list->header.nextElement = NULL;
    list->values.operator = EMPTY;
    list->values.operand = NULL;
    return list;
}
void addElement(struct dictLL *list,struct linkedListValues values){
    struct dictLL *newElement = malloc(sizeof(struct dictLL));
    struct dictLL *lastElement = list;
    while(lastElement->header.nextElement != NULL)
        lastElement = lastElement->header.nextElement;
    newElement->header.previousElement = lastElement;
    newElement->header.nextElement = newElement;

    newElement->values.operator = values.operator;
    newElement->values.operand = values.operand;
}

int isOperator(int value){
    switch(value){
        case DICTOPERAND_VERSION: 
        case DICTOPERAND_NOTICE: 
        case DICTOPERAND_FULLNAME: 
        case DICTOPERAND_FAMILYNAME: 
        case DICTOPERAND_WEIGHT: 
        case DICTOPERAND_FONTBBOX: 
        case DICTOPERAND_UNIQUEID: 
        case DICTOPERAND_XUID: 
        case DICTOPERAND_CHARSET: 
        case DICTOPERAND_ENCODING: 
        case DICTOPERAND_CHARSTRINGS: 
        case DICTOPERAND_PRIVATE: 
            return 1;
        case DICTOPERAND_DOUBLEBYTE:
            return 2;
        default :
            return 0;
    }
}