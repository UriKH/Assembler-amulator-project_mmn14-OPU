/* [mmn 14 : symTabTypes.h]:
In this file: type and data defenition for later use in the symbol-Table managing functions.

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon 
*/
#ifndef SYMTABTYPES_H
#define SYMTABTYPES_H

#define MAX_LABEL_LEN 31
enum attribs { EXT = 1, ENT, DAT, COD }; /* the types of directive commands in the Symbol Table attribute field */

typedef struct {
    char symbol[MAX_LABEL_LEN + 1];
    int address;
    enum attribs attribute1;
    enum attribs attribute2;
} symbolTable;

#endif